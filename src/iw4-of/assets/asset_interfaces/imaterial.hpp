#pragma once

#include <filesystem>

#include <rapidjson/document.h>

#include <game/structs.hpp>

#include "../asset_interface.hpp"
#include "../assets.hpp"

namespace iw4of::interfaces
{
  struct imaterial : asset_interface
  {
   public:
    imaterial(const iw4of::assets* assets)
        : asset_interface(assets)
    {
    }

   protected:
    bool write_internal(const native::XAssetHeader& header) const override;
    void* read_internal(const std::string& name) const override;

    std::filesystem::path get_file_name(const std::string& basename) const override;
    std::filesystem::path get_file_name(const native::XAssetHeader& header) const override
    {
      return get_file_name(header.material->info.name);
    };

    std::filesystem::path get_folder_name() const override
    {
      return "materials";
    }

   private:
    rapidjson::Value statebits_to_json_array(native::GfxStateBits* stateBits, uint8_t count,
                                             rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator>& allocator) const;
  };

} // namespace iw4of::interfaces