#include <std_include.hpp>

#include "assets/assets.hpp"
#include "iphyscollmap.hpp"

#include <utils/io.hpp>
#include <utils/string.hpp>

#include <utils/json.hpp>
#include <rapidjson/prettywriter.h>

#define IW4X_PHYS_COLLMAP_VERSION 1

namespace iw4of::interfaces
{
    bool iphyscollmap::write_internal(const native::XAssetHeader& header) const
    {
        rapidjson::Document output(rapidjson::kObjectType);
        auto& allocator = output.GetAllocator();

        auto asset = header.physCollmap;
        assert(asset);

        output.AddMember("version", IW4X_PHYS_COLLMAP_VERSION, allocator);
        output.AddMember("name", RAPIDJSON_STR(asset->name), allocator);

        rapidjson::Value geoms(rapidjson::kArrayType);
        for (size_t i = 0; i < asset->count; i++)
        {
            rapidjson::Value geom_info(rapidjson::kObjectType);
            const auto geom = &asset->geoms[i];

            {
                rapidjson::Value json_brush_wrapper(rapidjson::kObjectType);
                const auto brush_wrapper = geom->brushWrapper;

                json_brush_wrapper.AddMember("bounds", utils::json::to_json(brush_wrapper->bounds, allocator), allocator);
                json_brush_wrapper.AddMember("planes", utils::json::to_json(brush_wrapper->planes, brush_wrapper->brush.numsides, allocator), allocator);

				geom_info.AddMember("brushwrapper", json_brush_wrapper, allocator);
			}

            rapidjson::Value orientation(rapidjson::kArrayType);
            for (size_t j = 0; j < 3; j++)
            {
                orientation.PushBack(utils::json::make_json_array(geom->orientation[j], 3, allocator), allocator);
            }

            geom_info.AddMember("type", static_cast<int>(geom->type), allocator);
            geom_info.AddMember("orientation", orientation, allocator);
            geom_info.AddMember("bounds", utils::json::to_json(geom->bounds, allocator), allocator);
        }

        rapidjson::StringBuffer buff;
        rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buff);
        output.Accept(writer);

        const auto& dump = buff.GetString();

        return utils::io::write_file(get_work_path(header).string(), dump);
    }

    void* iphyscollmap::read_internal(const std::string& name) const
    {
        const auto& path = get_work_path(name).string();

        if (utils::io::file_exists(path))
        {
            auto asset = local_allocator.allocate<native::PhysPreset>();
            rapidjson::Document physPresetJson;

            try
            {
                const auto& content = utils::io::read_file(path);
                physPresetJson.Parse(content.data());
            }
            catch (const std::exception& e)
            {
                print_error("Invalid JSON for physpreset {}! {}", name, e.what());
                return nullptr;
            }

            try
            {
                // Must NOT be null!
                assert(!physPresetJson["sndAliasPrefix"].IsNull());

                asset->name = local_allocator.duplicate_string(physPresetJson["name"].GetString());
                asset->type = physPresetJson["type"].Get<int32_t>();
                asset->bounce = physPresetJson["bounce"].Get<float>();
                asset->mass = physPresetJson["mass"].Get<float>();
                asset->friction = physPresetJson["friction"].Get<float>();
                asset->bulletForceScale = physPresetJson["bulletForceScale"].Get<float>();
                asset->explosiveForceScale = physPresetJson["explosiveForceScale"].Get<float>();
                asset->sndAliasPrefix = local_allocator.duplicate_string(physPresetJson["sndAliasPrefix"].GetString());
                asset->piecesSpreadFraction = physPresetJson["piecesSpreadFraction"].Get<float>();
                asset->piecesUpwardVelocity = physPresetJson["piecesUpwardVelocity"].Get<float>();
                asset->tempDefaultToCylinder = physPresetJson["tempDefaultToCylinder"].Get<bool>();
                asset->perSurfaceSndAlias = physPresetJson["perSurfaceSndAlias"].Get<bool>();

                assert(asset->mass > FLT_EPSILON);
            }
            catch (const std::exception& e)
            {
                print_error("Malformed JSON for physpreset {}! {}", name, e.what());
                return nullptr;
            }

            return asset;
        }

        return nullptr;
    }

    std::filesystem::path iphyscollmap::get_file_name(const std::string& basename) const
    {
        return std::format("{}.iw4x.json", basename);
    }

    std::filesystem::path iphyscollmap::get_folder_name() const
    {
        return "physcollmap";
    }

} // namespace iw4of::interfaces
