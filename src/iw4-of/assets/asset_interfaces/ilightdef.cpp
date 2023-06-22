#include <std_include.hpp>

#include "ilightdef.hpp"

#include <utils/io.hpp>
#include <utils/stream.hpp>
#include <utils/string.hpp>

#include <rapidjson/document.h>
#include <rapidjson/prettywriter.h>

#define IW4X_LIGHT_VERSION "0"

namespace iw4of::interfaces
{
  bool interfaces::ilightdef::write_internal(
      const native::XAssetHeader& header) const
  {
    auto asset = header.lightDef;

    utils::stream buffer;
    buffer.save_array("IW4xLit" IW4X_LIGHT_VERSION, 8);

    buffer.save_object(*asset);

    if (asset->name)
    {
      buffer.save_string(asset->name);
    }

    if (asset->attenuation.image)
    {
      buffer.save_string(asset->attenuation.image->name);

      if (!assets->write(native::XAssetType::ASSET_TYPE_IMAGE,
                         asset->attenuation.image))
      {
        return false;
      }
    }

    RETURN_IF_NULL(asset->name);

    if (asset->name)
    {
      return utils::io::write_file(get_work_path(header).string(),
                                   buffer.to_buffer());
    }

    return false;
  }

  void* interfaces::ilightdef::read_internal(const std::string& name) const
  {
    const auto& path = get_work_path(name).string();

    if (utils::io::file_exists(path))
    {
      auto contents = utils::io::read_file(path);

      utils::stream::reader reader(&local_allocator, contents);

      char* magic = reader.read_array<char>(7);
      if (std::memcmp(magic, "IW4xLit", 7))
      {
        print_error("Reading light '{}' failed, header is invalid!", name);
        return nullptr;
      }

      std::string version;
      version.push_back(reader.read<char>());
      if (version != IW4X_LIGHT_VERSION)
      {
        print_error(
            "Reading light '{}' failed, expected version is {}, but it was {}!",
            name,
            IW4X_LIGHT_VERSION,
            version);
        return nullptr;
      }

      native::GfxLightDef* asset = reader.read_object<native::GfxLightDef>();

      if (asset->name)
      {
        asset->name = reader.read_cstring();
      }

      if (asset->attenuation.image)
      {
        asset->attenuation.image = find<native::GfxImage>(
            native::XAssetType::ASSET_TYPE_IMAGE, reader.read_string());
      }

      return asset;
    }

    return nullptr;
  }

  std::filesystem::path interfaces::ilightdef::get_file_name(
      const std::string& basename) const
  {
    return std::format("{}.iw4xLight", basename);
  }
} // namespace iw4of::interfaces
