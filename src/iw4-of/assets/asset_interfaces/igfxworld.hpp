#pragma once

#include <utils/stream.hpp>

namespace iw4of::interfaces
{
  struct igfxworld : asset_interface
  {
   public:
    igfxworld(const iw4of::assets* assets)
        : asset_interface(assets)
    {
    }

   protected:
    bool write_internal(const native::XAssetHeader& header) const override;
    void* read_internal(const std::string& name) const override;
    std::filesystem::path get_file_name(const std::string& basename) const override;
    std::filesystem::path get_folder_name() const override;

   private:
    void write(const native::GfxWorldDraw* dpvs_static, utils::stream* buffer) const;
    void write(const native::GfxWorld* world, const native::GfxWorldDpvsStatic* dpvs_static, utils::stream* buffer) const;
    void write(const native::GfxLightGrid* asset, utils::stream* buffer) const;
    void write(const native::GfxWorld* world, const native::GfxWorldDpvsPlanes* asset, utils::stream* buffer) const;

    bool read_dpvs_static(const native::GfxWorld* world, native::GfxWorldDpvsStatic* asset, utils::stream::reader* reader) const;
    bool read_gfx_world_draw(native::GfxWorldDraw* asset, utils::stream::reader* reader) const;
  };
} // namespace iw4of::interfaces