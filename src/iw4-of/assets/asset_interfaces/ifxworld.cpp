#include <std_include.hpp>

#include "ifxworld.hpp"

#include "utils/io.hpp"
#include "utils/stream.hpp"
#include "utils/string.hpp"
#include "utils/json.hpp"

#include "rapidjson/prettywriter.h"

#define IW4X_FXWORLD_VERSION 1

namespace iw4of::interfaces
{


	bool ifxworld::write_internal(const native::XAssetHeader& header) const
	{
		auto asset = header.fxWorld;

		rapidjson::Document output(rapidjson::kObjectType);
		auto& allocator = output.GetAllocator();
		utils::memory::allocator str_duplicator;

		output.AddMember("version", IW4X_FXWORLD_VERSION, allocator);
		output.AddMember("name", RAPIDJSON_STR(asset->name), allocator);


		rapidjson::Value glass_system(rapidjson::kObjectType);

#define SAME_NAME_JSON_MEMBER(obj, json, value) json.AddMember(#value, obj.value, allocator)

		SAME_NAME_JSON_MEMBER(asset->glassSys, glass_system, time);
		SAME_NAME_JSON_MEMBER(asset->glassSys, glass_system, prevTime);
		SAME_NAME_JSON_MEMBER(asset->glassSys, glass_system, defCount);
		SAME_NAME_JSON_MEMBER(asset->glassSys, glass_system, pieceLimit);
		SAME_NAME_JSON_MEMBER(asset->glassSys, glass_system, pieceWordCount);
		SAME_NAME_JSON_MEMBER(asset->glassSys, glass_system, initPieceCount);
		SAME_NAME_JSON_MEMBER(asset->glassSys, glass_system, cellCount);
		SAME_NAME_JSON_MEMBER(asset->glassSys, glass_system, activePieceCount);
		SAME_NAME_JSON_MEMBER(asset->glassSys, glass_system, firstFreePiece);
		SAME_NAME_JSON_MEMBER(asset->glassSys, glass_system, geoDataLimit);
		SAME_NAME_JSON_MEMBER(asset->glassSys, glass_system, geoDataCount);
		SAME_NAME_JSON_MEMBER(asset->glassSys, glass_system, initGeoDataCount);

		rapidjson::Value defs(rapidjson::kArrayType);
		for (size_t i = 0; i < asset->glassSys.defCount; i++)
		{
			auto def = &asset->glassSys.defs[i];

			rapidjson::Value def_json(rapidjson::kObjectType);

			def_json.AddMember("halfThickness", def->halfThickness, allocator);

			rapidjson::Value texvecs_array(rapidjson::kArrayType);
			texvecs_array.PushBack(def->texVecs[0][0], allocator);
			texvecs_array.PushBack(def->texVecs[0][1], allocator);
			texvecs_array.PushBack(def->texVecs[1][0], allocator);
			texvecs_array.PushBack(def->texVecs[1][1], allocator);

			def_json.AddMember("texVecs", texvecs_array, allocator);

			def_json.AddMember("color", def->color.packed, allocator);
			def_json.AddMember("material", RAPIDJSON_STR(def->material->info.name), allocator);
			def_json.AddMember("materialShattered", RAPIDJSON_STR(def->materialShattered->info.name), allocator);
			def_json.AddMember("physPreset", RAPIDJSON_STR(def->physPreset->name), allocator);

			defs.PushBack(def_json, allocator);
		}

		glass_system.AddMember("defs", defs, allocator);

		rapidjson::Value init_pieces(rapidjson::kArrayType);
		for (size_t i = 0; i < asset->glassSys.initPieceCount; i++)
		{
			auto init_piece_state = &asset->glassSys.initPieceStates[i];

			rapidjson::Value init_piece_json(rapidjson::kObjectType);

			rapidjson::Value fx_spatial_frame(rapidjson::kObjectType);

			rapidjson::Value fx_quat(rapidjson::kArrayType);
			for (size_t j = 0; j < ARRAYSIZE(init_piece_state->frame.quat); j++)
			{
				fx_quat.PushBack(init_piece_state->frame.quat[j], allocator);
			}

			rapidjson::Value fx_origin(rapidjson::kArrayType);
			for (size_t j = 0; j < ARRAYSIZE(init_piece_state->frame.origin); j++)
			{
				fx_origin.PushBack(init_piece_state->frame.origin[j], allocator);
			}

			fx_spatial_frame.AddMember("quat", fx_quat, allocator);
			fx_spatial_frame.AddMember("origin", fx_origin, allocator);

			init_piece_json.AddMember("frame", fx_spatial_frame, allocator);
			init_piece_json.AddMember("radius", init_piece_state->radius, allocator);

			rapidjson::Value tex_coord_origin(rapidjson::kArrayType);
			tex_coord_origin.PushBack(init_piece_state->texCoordOrigin[0],allocator);
			tex_coord_origin.PushBack(init_piece_state->texCoordOrigin[1],allocator);
			init_piece_json.AddMember("texCoordOrigin", tex_coord_origin, allocator);

			init_piece_json.AddMember("supportMask", init_piece_state->supportMask, allocator);
			init_piece_json.AddMember("areaX2", init_piece_state->areaX2, allocator);
			init_piece_json.AddMember("defIndex", init_piece_state->defIndex, allocator);
			init_piece_json.AddMember("vertCount", init_piece_state->vertCount, allocator);
			init_piece_json.AddMember("fanDataCount", init_piece_state->fanDataCount, allocator);
		
			init_pieces.PushBack(init_piece_json, allocator);
		}

		glass_system.AddMember("initPieceStates", init_pieces, allocator);


		rapidjson::Value init_geo_json(rapidjson::kArrayType);
		for (size_t i = 0; i < asset->glassSys.initGeoDataCount; i++)
		{
			auto init_geo = &asset->glassSys.initGeoData[i];

			rapidjson::Value data_array(rapidjson::kArrayType);
			data_array.PushBack(init_geo->anonymous[0], allocator);
			data_array.PushBack(init_geo->anonymous[1], allocator);

			init_geo_json.PushBack(data_array, allocator);
		}

		glass_system.AddMember("initGeoData", init_geo_json, allocator);

		// And that's it! Everything else in this thing is RUNTIME!
		// Crazy huh ?

		output.AddMember("glassSys", glass_system, allocator);

		rapidjson::StringBuffer buff;
		rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buff);
		output.Accept(writer);

		const auto& dump = buff.GetString();

		return utils::io::write_file(get_work_path(header).string(), dump);
	}

	void* interfaces::ifxworld::read_internal(const std::string& name) const
	{
		const auto& path = get_work_path(name).string();

		if (!utils::io::file_exists(path))
		{
			return nullptr;
		}

		rapidjson::Document fxWorldJson;

		try
		{
			const auto& contents = utils::io::read_file(path);
			fxWorldJson.Parse(contents.data());
		}
		catch (const std::exception& e)
		{
			print_error("Invalid JSON for gameworld {}! {}", name, e.what());
			return nullptr;
		}

		if (!fxWorldJson.IsObject())
		{
			print_error("Invalid FXWORLD json for {}\n", name);
			return nullptr;
		}

		auto version = fxWorldJson["version"].IsNumber() ? fxWorldJson["version"].Get<int32_t>() : 0;
		if (version > IW4X_FXWORLD_VERSION)
		{
			print_error("Invalid FXWORLD json version for {}, expected {} and got {}\n", name, IW4X_FXWORLD_VERSION, version);
			return nullptr;
		}

		auto map = local_allocator.allocate<native::FxWorld>();
		map->name = local_allocator.duplicate_string(name);

		try
		{
			auto glassSys = &map->glassSys;
			const auto& glassSysJson = fxWorldJson["glassSys"];

			glassSys->time = glassSysJson["time"].Get<int32_t>();
			glassSys->prevTime = glassSysJson["prevTime"].Get<int32_t>();
			glassSys->defCount = glassSysJson["defCount"].Get<uint32_t>();
			glassSys->pieceLimit = glassSysJson["pieceLimit"].Get<uint32_t>();
			glassSys->pieceWordCount = glassSysJson["pieceWordCount"].Get<uint32_t>();
			glassSys->initPieceCount = glassSysJson["initPieceCount"].Get<uint32_t>();
			glassSys->cellCount = glassSysJson["cellCount"].Get<uint32_t>();
			glassSys->activePieceCount = glassSysJson["activePieceCount"].Get<uint32_t>();
			glassSys->firstFreePiece = glassSysJson["firstFreePiece"].Get<uint32_t>();
			glassSys->geoDataLimit = glassSysJson["geoDataLimit"].Get<uint32_t>();
			glassSys->geoDataCount = glassSysJson["geoDataCount"].Get<uint32_t>();
			glassSys->initGeoDataCount = glassSysJson["initGeoDataCount"].Get<uint32_t>();

			glassSys->defs = local_allocator.allocate_array<native::FxGlassDef>(glassSys->defCount);
			for (size_t i = 0; i < glassSysJson["defs"].Size(); i++)
			{
				const auto& member = glassSysJson["defs"][i];
				auto def = &glassSys->defs[i];

				def->halfThickness = member["halfThickness"].Get<float>();

				auto xy = 0;
				for (auto x = 0; x < 2; x++)
				{
					for (auto y = 0; y < 2; y++)
					{
						def->texVecs[x][y] = member["texVecs"][xy].Get<float>();
						xy++;
					}
				}

				def->color.packed = member["color"].Get<int32_t>();

				auto matShateredName = member["materialShattered"].Get<std::string>();
				auto matName = member["material"].Get<std::string>();
				auto physPresetName = member["physPreset"].Get<std::string>();
				def->material = find<native::Material>(native::ASSET_TYPE_MATERIAL, matName);
				def->materialShattered = find<native::Material>(native::ASSET_TYPE_MATERIAL, matShateredName);
				def->physPreset = find<native::PhysPreset>(native::ASSET_TYPE_PHYSPRESET, physPresetName);

				RETURN_IF_NULL(def->material);
				RETURN_IF_NULL(def->materialShattered);
				RETURN_IF_NULL(def->physPreset);
			}

			glassSys->initPieceStates = local_allocator.allocate_array<native::FxGlassInitPieceState>(glassSys->initPieceCount);
			for (size_t i = 0; i < glassSysJson["initPieceStates"].Size(); i++)
			{
				const auto& member = glassSysJson["initPieceStates"][i];
				auto initial = &glassSys->initPieceStates[i];

				for (int j = 0; j < ARRAYSIZE(initial->frame.quat); j++)
				{
					initial->frame.quat[j] = member["frame"]["quat"][j].Get<float>();
				}

				for (int j = 0; j < ARRAYSIZE(initial->frame.origin); j++)
				{
					initial->frame.origin[j] = member["frame"]["origin"][j].Get<float>();
				}

				initial->radius = member["radius"].Get<float>();
				initial->texCoordOrigin[0] = member["texCoordOrigin"][0].Get<float>();
				initial->texCoordOrigin[1] = member["texCoordOrigin"][1].Get<float>();
				initial->supportMask = member["supportMask"].Get<int32_t>();
				initial->areaX2 = member["areaX2"].Get<float>();
				initial->defIndex = member["defIndex"].Get<char>();
				initial->vertCount = member["vertCount"].Get<char>();
				initial->fanDataCount = member["fanDataCount"].Get<char>();
			}

			glassSys->initGeoData = local_allocator.allocate_array<native::FxGlassGeometryData>(glassSys->initGeoDataCount);
			for (size_t i = 0; i < glassSysJson["initGeoData"].Size(); i++)
			{
				const auto& member = glassSysJson["initGeoData"][i];
				auto data = &glassSys->initGeoData[i];
				data->anonymous[0] = member[0].Get<int16_t>();
				data->anonymous[1] = member[1].Get<int16_t>();
			}
		}
		catch (const std::exception& e)
		{
			print_error("Malformed FXWORLD json for {} ({})\n", name, e.what());
			return nullptr;
		}

		map->glassSys.piecePlaces = local_allocator.allocate_array<native::FxGlassPiecePlace>(map->glassSys.pieceLimit);
		map->glassSys.pieceStates = local_allocator.allocate_array<native::FxGlassPieceState>(map->glassSys.pieceLimit);
		map->glassSys.pieceDynamics = local_allocator.allocate_array<native::FxGlassPieceDynamics>(map->glassSys.pieceLimit);
		map->glassSys.geoData = local_allocator.allocate_array<native::FxGlassGeometryData>(map->glassSys.geoDataLimit);
		map->glassSys.isInUse = local_allocator.allocate_array<uint32_t>(map->glassSys.pieceWordCount);
		map->glassSys.cellBits = local_allocator.allocate_array<uint32_t>(map->glassSys.pieceWordCount * map->glassSys.cellCount);
		map->glassSys.visData = local_allocator.allocate_array<uint8_t>((map->glassSys.pieceLimit + 15) & 0xFFFFFFF0); // ugh
		map->glassSys.linkOrg = reinterpret_cast<float(*)[3]>(local_allocator.allocate_array<float>(map->glassSys.pieceLimit));
		map->glassSys.halfThickness = local_allocator.allocate_array<float>(map->glassSys.pieceLimit * 3);
		map->glassSys.lightingHandles = local_allocator.allocate_array<uint16_t>(map->glassSys.initPieceCount);

		return map;
	}

	std::filesystem::path interfaces::ifxworld::get_file_name(const std::string& name) const
	{
		constexpr auto prefix = "maps/mp/";
		constexpr auto suffix = ".d3dbsp";

		std::string basename = name;
		utils::string::replace(basename, prefix, "");
		utils::string::replace(basename, suffix, "");

		return std::format("{}.iw4x.json", basename);
	}
}