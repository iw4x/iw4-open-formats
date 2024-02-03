#pragma once

#include <filesystem>

#include <game/structs.hpp>

#include "../asset_interface.hpp"
#include "../assets.hpp"

namespace iw4of::interfaces
{
    struct irawfile : asset_interface
    {
       public:
        irawfile(const iw4of::assets* assets)
            : asset_interface(assets)
        {
        }

       protected:
        bool write_internal(const native::XAssetHeader& header) const override;
        void* read_internal(const std::string& name) const override;
        std::vector<native::XAsset> get_child_assets(const native::XAssetHeader& header) const;

        std::filesystem::path get_file_name(const std::string& basename) const override;
        std::filesystem::path get_folder_name() const override
        {
            return std::filesystem::path{}; // Empty path
        }

       private:
        void write_named_assets(const std::string& script_name, const std::string& script_contents) const;
        void read_named_assets(const std::string& script_name, const std::string& script_contents) const;
		
        std::vector<native::XAsset> get_child_assets(const std::string& script_name, const std::string& script_contents) const;
		std::vector<native::XAnimParts*> get_animtree_anims(const std::string& script) const;
        std::vector<native::RawFile*> get_map_animtrees(const std::string& script) const;
        std::vector<native::XAnimParts*> get_map_animated_model_anims(const std::string& script) const;
        native::snd_alias_list_t* get_ambient_play(const std::string& script) const;
        std::vector<native::FxEffectDef*> get_map_fx(const std::string& script) const;
        std::vector<native::XAsset> get_assets_in_precache(const std::string& script_contents) const;
        std::vector<native::snd_alias_list_t*> get_create_fx_sounds(const std::string& script) const;
    };
} // namespace iw4of::interfaces
