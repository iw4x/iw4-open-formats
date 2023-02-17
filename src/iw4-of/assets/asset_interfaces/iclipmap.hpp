#pragma once

#include <filesystem>

#include <game/structs.hpp>

#include "../asset_interface.hpp"
#include "../assets.hpp"

namespace iw4of::interfaces
{
  struct iclipmap : asset_interface
  {
   public:
    iclipmap(const iw4of::assets* assets)
        : asset_interface(assets)
    {
    }

   protected:
    bool write_internal(const native::XAssetHeader& header) const override;
    void* read_internal(const std::string& name) const override;
    bool write_plaintext(const native::XAssetHeader& header) const;

    std::filesystem::path get_file_name(const std::string& basename) const override;
    std::filesystem::path get_folder_name() const override
    {
      return "clipmap";
    }
  };
} // namespace iw4of::interfaces