#pragma once

#include <filesystem>

#include <game/structs.hpp>

#include "../asset_interface.hpp"
#include "../assets.hpp"

namespace iw4of::interfaces
{
    struct ifxworld : asset_interface
    {
       public:
        ifxworld(const iw4of::assets* assets)
            : asset_interface(assets)
        {
        }

       protected:
        std::vector<native::XAsset> get_child_assets(const native::XAssetHeader& header) const;
        bool write_internal(const native::XAssetHeader& header) const override;
        void* read_internal(const std::string& name) const override;

        std::filesystem::path get_file_name(const std::string& basename) const override;
        std::filesystem::path get_folder_name() const override
        {
            return "fxworld";
        }
    };
} // namespace iw4of::interfaces
