#pragma once

#include "assets/asset_interface.hpp"

namespace iw4of::interfaces
{
    struct isndalias : asset_interface
    {
       public:
        isndalias(const iw4of::assets* assets)
            : asset_interface(assets)
        {
        }

       protected:
        bool write_internal(const native::XAssetHeader& header) const override;
        void* read_internal(const std::string& name) const override;
        std::vector<native::XAsset> get_child_assets(const native::XAssetHeader& header) const;

        std::filesystem::path get_file_name(const std::string& basename) const override;
        std::filesystem::path get_folder_name() const override;
    };
} // namespace iw4of::interfaces
