#pragma once

#include "assets/asset_interface.hpp"

namespace iw4of::interfaces
{
    struct imapents : asset_interface
    {
       public:
        imapents(const iw4of::assets* assets)
            : asset_interface(assets)
        {
        }

       protected:
        bool write_internal(const native::XAssetHeader& header) const override;
        void* read_internal(const std::string& name) const override;
        std::filesystem::path get_file_name(const std::string& basename) const override;
        std::filesystem::path get_folder_name() const override;
        std::vector<native::XAsset> get_child_assets(const native::XAssetHeader& header) const;

       private:
        std::vector<native::XAsset> get_assets(const std::string& entity_string) const;
        std::vector<native::WeaponCompleteDef*> get_weapons(const std::string& entity_string) const;
        std::vector<native::XModel*> get_models(const std::string& entity_string) const;
    };
} // namespace iw4of::interfaces
