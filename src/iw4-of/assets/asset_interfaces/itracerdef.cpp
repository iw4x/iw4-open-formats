#include <std_include.hpp>

#include "itracerdef.hpp"

#include <utils/io.hpp>
#include <utils/stream.hpp>
#include <utils/string.hpp>
#include <utils/json.hpp>

#include <rapidjson/prettywriter.h>

namespace iw4of::interfaces
{

#define IW4X_TRACERDEF_VERSION 1

    bool itracerdef::write_internal(const native::XAssetHeader& header) const
    {
        rapidjson::Document output(rapidjson::kObjectType);
        auto& allocator = output.GetAllocator();

        auto asset = header.tracerDef;

        output.AddMember("version", IW4X_TRACERDEF_VERSION, allocator);

        output.AddMember("name", RAPIDJSON_STR(asset->name), allocator);

		assets->write(native::ASSET_TYPE_MATERIAL, asset->material);
        output.AddMember("material", RAPIDJSON_STR(asset->material->info.name), allocator);

        output.AddMember("drawInterval", asset->drawInterval, allocator);
        output.AddMember("speed", asset->speed, allocator);
        output.AddMember("beamLength", asset->beamLength, allocator);
        output.AddMember("beamWidth", asset->beamWidth, allocator);
        output.AddMember("screwRadius", asset->screwRadius, allocator);
        output.AddMember("screwDist", asset->screwDist, allocator);

		rapidjson::Value colors(rapidjson::kArrayType);
		for (size_t i = 0; i < 5; i++)
		{
			colors.PushBack(utils::json::make_json_array(asset->colors[i], 4, allocator), allocator);
		}

		output.AddMember("colors", colors, allocator);

        rapidjson::StringBuffer buff;
        rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buff);
        output.Accept(writer);

        const auto& dump = buff.GetString();

        return utils::io::write_file(get_work_path(header).string(), dump);
    }

    void* itracerdef::read_internal(const std::string& name) const
    {
        const auto& path = get_work_path(name).string();

        if (utils::io::file_exists(path))
        {
            auto asset = local_allocator.allocate<native::TracerDef>();
            rapidjson::Document tracer_def_json;

            try
            {
                const auto& content = utils::io::read_file(path);
                tracer_def_json.Parse(content.data());
            }
            catch (const std::exception& e)
            {
                print_error("Invalid JSON for tracerdef {}! {}", name, e.what());
                return nullptr;
            }

            try
            {
                asset->name = local_allocator.duplicate_string(tracer_def_json["name"].GetString());
                asset->drawInterval = tracer_def_json["drawInterval"].Get<uint32_t>();
                asset->speed = tracer_def_json["speed"].Get<float>();
                asset->beamLength = tracer_def_json["beamLength"].Get<float>();
                asset->beamWidth = tracer_def_json["beamWidth"].Get<float>();
                asset->screwRadius = tracer_def_json["screwRadius"].Get<float>();
                asset->screwDist = tracer_def_json["screwDist"].Get<float>();
                asset->material = find<native::Material>(native::ASSET_TYPE_MATERIAL, tracer_def_json["material"].GetString());

				
				for (size_t i = 0; i < 5; i++)
				{
					for (size_t j = 0; j < 4; j++)
					{
						asset->colors[i][j] =tracer_def_json["colors"][i][j].Get<float>();
					}
				}
            }
            catch (const std::exception& e)
            {
                print_error("Malformed JSON for tracerdef {}! {}", name, e.what());
                return nullptr;
            }

            return asset;
        }

        return nullptr;
    }

    std::filesystem::path itracerdef::get_file_name(const std::string& basename) const
    {
        return std::format("{}.iw4x.json", basename);
    }

} // namespace iw4of::interfaces
