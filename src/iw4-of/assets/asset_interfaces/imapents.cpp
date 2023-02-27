#include <std_include.hpp>

#include "imapents.hpp"
#include "assets/assets.hpp"

#include <utils/io.hpp>
#include <utils/stream.hpp>
#include <utils/string.hpp>

#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>

namespace iw4of::interfaces
{
    bool imapents::write_internal(const native::XAssetHeader& header) const
    {
        std::string basename(header.mapEnts->name);
        basename.erase(0, 8);
        basename.erase(basename.end() - 7, basename.end());

        auto asset_list = get_assets(header.mapEnts->entityString);

        for (auto asset : asset_list)
        {
            assets->write(asset.type, asset.header.data);
        }

        return utils::io::write_file(get_work_path(header).string(), header.mapEnts->entityString);
    }

    void* iw4of::interfaces::imapents::read_internal(const std::string& name) const
    {
        const auto& path = get_work_path(name).string();

        if (utils::io::file_exists(path))
        {
            auto contents = utils::io::read_file(path);

            native::MapEnts* entities = local_allocator.allocate<native::MapEnts>();

            entities->stageCount = 1;
            entities->stages = local_allocator.allocate<native::Stage>();
            entities->stages[0].name = "stage 0";
            entities->stages[0].triggerIndex = 0x400;
            entities->stages[0].sunPrimaryLightIndex = 0x1;

            entities->name = local_allocator.duplicate_string(std::format("maps/mp/{}.d3dbsp", name));
            entities->entityString = local_allocator.duplicate_string(contents);
            entities->numEntityChars = contents.size() + 1;

            get_assets(contents);

            return entities;
        }

        return nullptr;
    }

    std::vector<native::XAsset> imapents::get_assets(const std::string& entity_string) const
    {
        std::vector<native::XAsset> asset_list{};

        for (auto xmodel : get_models(entity_string))
        {
            asset_list.push_back({native::ASSET_TYPE_XMODEL, xmodel});
        }

        return asset_list;
    }

    std::vector<native::XModel*> imapents::get_models(const std::string& entity_string) const
    {
        auto models = std::vector<native::XModel*>();

        std::regex model_catcher("model\"? \"([^\\*\\?].*)\"");

        std::smatch m;

        std::string::const_iterator search_start(entity_string.cbegin());
        while (std::regex_search(search_start, entity_string.cend(), m, model_catcher))
        {
            bool skip = true;
            for (auto match : m)
            {
                if (skip)
                {
                    skip = false;
                    continue;
                }

                auto model_name = match.str();
                auto model = find<native::XModel>(native::ASSET_TYPE_XMODEL, model_name);
                search_start = m.suffix().first;

                if (model)
                {
                    models.push_back(model);
                }
            }
        }

        return models;
    }

    std::filesystem::path iw4of::interfaces::imapents::get_file_name(const std::string& name) const
    {
        constexpr auto prefix = "maps/mp/";
        constexpr auto suffix = ".d3dbsp";

        std::string basename = name;
        utils::string::replace(basename, prefix, "");
        utils::string::replace(basename, suffix, "");

        return std::format("{}.ents", basename);
    }

    std::filesystem::path iw4of::interfaces::imapents::get_folder_name() const
    {
        return "mapents";
    }
} // namespace iw4of::interfaces