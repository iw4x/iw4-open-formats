#include <std_include.hpp>

#include "imapents.hpp"

#include <utils/io.hpp>
#include <utils/stream.hpp>
#include <utils/string.hpp>

#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>

namespace iw4of::interfaces
{
	bool iw4of::interfaces::imapents::write_internal(const native::XAssetHeader& header) const
	{
		std::string basename(header.mapEnts->name);
		basename.erase(0, 8);
		basename.erase(basename.end() - 7, basename.end());

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

			return entities;
		}

		return nullptr;
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
}