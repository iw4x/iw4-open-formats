#include <std_include.hpp>

#include "iphyspreset.hpp"

#include <utils/io.hpp>
#include <utils/stream.hpp>
#include <utils/string.hpp>
#include <utils/json.hpp>

#include <rapidjson/prettywriter.h>

namespace iw4of::interfaces
{

#define IW4X_PHYSPRESET_VERSION 1

  bool iphyspreset::write_internal(const native::XAssetHeader& header) const
  {
    rapidjson::Document output(rapidjson::kObjectType);
    auto& allocator = output.GetAllocator();

    auto asset = header.physPreset;

    output.AddMember("version", IW4X_PHYSPRESET_VERSION, allocator);

    output.AddMember("name", RAPIDJSON_STR(asset->name), allocator);

    output.AddMember("type", asset->type, allocator);
    output.AddMember("mass", asset->mass, allocator);
    output.AddMember("bounce", asset->bounce, allocator);
    output.AddMember("friction", asset->friction, allocator);
    output.AddMember("bulletForceScale", asset->bulletForceScale, allocator);
    output.AddMember("explosiveForceScale", asset->explosiveForceScale, allocator);
    output.AddMember("sndAliasPrefix", RAPIDJSON_STR(asset->sndAliasPrefix), allocator);
    output.AddMember("piecesSpreadFraction", asset->piecesSpreadFraction, allocator);
    output.AddMember("piecesUpwardVelocity", asset->piecesUpwardVelocity, allocator);
    output.AddMember("tempDefaultToCylinder", asset->tempDefaultToCylinder, allocator);
    output.AddMember("perSurfaceSndAlias", asset->perSurfaceSndAlias, allocator);

    rapidjson::StringBuffer buff;
    rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buff);
    output.Accept(writer);

    const auto& dump = buff.GetString();

    return utils::io::write_file(get_work_path(header).string(), dump);
  }

  void* iphyspreset::read_internal(const std::string& name) const
  {
    const auto& path = get_work_path(name).string();
    auto asset = local_allocator.allocate<native::PhysPreset>();

    if (utils::io::file_exists(path))
    {
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
    }

    return asset;
  }

  std::filesystem::path iphyspreset::get_file_name(const std::string& basename) const
  {
    return std::format("{}.iw4x.json", basename);
  }

} // namespace iw4of::interfaces
