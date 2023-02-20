#include <std_include.hpp>

#include "icomworld.hpp"

#include <utils/io.hpp>
#include <utils/stream.hpp>
#include <utils/string.hpp>
#include <utils/json.hpp>

#define IW4X_COMMAP_VERSION 0

namespace iw4of::interfaces
{
    bool interfaces::icomworld::write_internal(const native::XAssetHeader& header) const
    {
        auto asset = header.comWorld;

        utils::stream buffer;
        buffer.save_array("IW4xComW", 8);
        buffer.save_object(IW4X_COMMAP_VERSION);

        buffer.save_object(*asset);

        if (asset->name)
        {
            buffer.save_string(asset->name);
        }

        if (asset->primaryLights)
        {
            AssertSize(native::ComPrimaryLight, 68);

            buffer.save_array(asset->primaryLights, asset->primaryLightCount);

            for (uint32_t i = 0; i < asset->primaryLightCount; ++i)
            {
                native::ComPrimaryLight* light = &asset->primaryLights[i];

                if (light->defName)
                {
                    buffer.save_string(light->defName);
                    assets->write(native::XAssetType::ASSET_TYPE_LIGHT_DEF, light->defName);
                }
            }
        }

        assert(asset->name);

        if (asset->name)
        {
            std::string basename(asset->name);

            constexpr auto prefix = "maps/mp/";
            constexpr auto suffix = ".d3dbsp";

            basename = basename.substr(strlen(prefix), basename.size() - strlen(suffix) - strlen(prefix));

            return utils::io::write_file(get_work_path(header).string(), buffer.to_buffer());
        }

        return false;
    }

    void* interfaces::icomworld::read_internal(const std::string& name) const
    {
        const auto path = get_work_path(name).string();

        if (utils::io::file_exists(path))
        {
            auto contents = utils::io::read_file(path);
            utils::stream::reader reader(&local_allocator, contents);

            int64_t magic = reader.read<int64_t>();
            if (std::memcmp(&magic, "IW4xComW", 8))
            {
                print_error("Reading comworld '{}' failed, header is invalid!", name);
                return nullptr;
            }

            int32_t version = reader.read<int32_t>();
            if (version > IW4X_COMMAP_VERSION)
            {
                print_error("Reading comworld '{}' failed, expected version is {}, but it was {}!", name, IW4X_COMMAP_VERSION, version);
                return nullptr;
            }

            native::ComWorld* asset = reader.read_object<native::ComWorld>();

            if (asset->name)
            {
                asset->name = reader.read_cstring();
            }

            if (asset->primaryLights)
            {
                asset->primaryLights = reader.read_array<native::ComPrimaryLight>(asset->primaryLightCount);

                for (uint32_t i = 0; i < asset->primaryLightCount; ++i)
                {
                    native::ComPrimaryLight* light = &asset->primaryLights[i];

                    if (light->defName)
                    {
                        light->defName = reader.read_cstring();
                        const auto& light_asset = find<void>(native::XAssetType::ASSET_TYPE_LIGHT_DEF, light->defName);

                        RETURN_IF_NULL(light_asset);
                    }
                }
            }

            return asset;
        }

        return nullptr;
    }

    std::filesystem::path interfaces::icomworld::get_file_name(const std::string& name) const
    {
        constexpr auto prefix = "maps/mp/";
        constexpr auto suffix = ".d3dbsp";

        std::string basename = name;
        utils::string::replace(basename, prefix, "");
        utils::string::replace(basename, suffix, "");

        return std::format("{}.iw4xComWorld", basename);
    }
} // namespace iw4of::interfaces