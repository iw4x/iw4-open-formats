#pragma once

#include <game/structs.hpp>

#include "../asset_interface.hpp"
#include "../assets.hpp"

namespace iw4of::interfaces
{
  struct igfximage : asset_interface
  {
   public:
    igfximage(const iw4of::assets* assets)
        : asset_interface(assets)
    {
    }

   protected:
    bool write_internal(const native::XAssetHeader& header) const override;
    void* read_internal(const std::string& name) const override;

    std::filesystem::path get_file_name(
        const native::XAssetHeader& header) const override;
    std::filesystem::path get_file_name(
        const std::string& basename) const override;
    std::filesystem::path get_folder_name() const override
    {
      return "images";
    }

   private:
    bool is_special(iw4of::native::GfxImage* image) const
    {
      return image != nullptr &&
             image->category !=
                 native::GfxImageCategory::IMG_CATEGORY_LOAD_FROM_FILE &&
             image->texture.loadDef;
    }

    std::filesystem::path get_special_file_name(
        const std::string& basename) const;
  };
} // namespace iw4of::interfaces