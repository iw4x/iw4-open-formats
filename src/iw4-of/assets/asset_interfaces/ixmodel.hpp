#pragma once

#include <utils/stream.hpp>
#include "assets/asset_interface.hpp"

namespace iw4of::interfaces
{
    struct ixmodel : asset_interface
    {
       public:
        ixmodel(const iw4of::assets* assets)
            : asset_interface(assets)
        {
        }

       protected:
		   std::vector<native::XAsset> get_child_assets(const native::XAssetHeader& header) const override;
		   bool write_internal(const native::XAssetHeader& header) const override;
        void* read_internal(const std::string& name) const override;
        std::filesystem::path get_file_name(const std::string& basename) const override;
        std::filesystem::path get_folder_name() const override;

       private:
        void write(const native::XSurfaceCollisionTree* entry, utils::stream* buffer) const;
        void write(const native::XSurface* surf, utils::stream* buffer) const;
        void write(const native::XModelSurfs* asset, utils::stream* buffer) const;
        void read(native::XSurfaceCollisionTree* entry, utils::stream::reader* reader) const;
        void read(native::XSurface* asset, utils::stream::reader* reader) const;
        void read(native::XModelSurfs* asset, utils::stream::reader* reader) const;
    };
} // namespace iw4of::interfaces
