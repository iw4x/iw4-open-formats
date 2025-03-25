#pragma once

#include <filesystem>

#include <rapidjson/document.h>

#include <game/structs.hpp>

#include "../asset_interface.hpp"
#include "../assets.hpp"

namespace iw4of::interfaces
{
    struct iweapon : asset_interface
    {
       public:
        iweapon(const iw4of::assets* assets)
            : asset_interface(assets)
        {
        }

       protected:
        bool write_internal(const native::XAssetHeader& header) const override;
        void* read_internal(const std::string& name) const override;
        std::vector<native::XAsset> get_child_assets(const native::XAssetHeader& header) const override;

        std::filesystem::path get_file_name(const std::string& basename) const override;
        std::filesystem::path get_file_name(const native::XAssetHeader& header) const override
        {
            return get_file_name(header.weapon->szInternalName);
        };

        std::filesystem::path get_folder_name() const override
        {
            return "weapons";
        }

       private:
        const std::vector<std::string> weapon_attachments = {
            "acog",
            "akimbo",
            "eotech",
            "fmj",
            "gl",
            "grip",
            "heartbeat",
            "reflex",
            "rof",
            "scope",
            "shotgun",
            "silencer",
            "tactical",
            "thermal",
            "xmags",
        };

        void write_xmodels_array(const std::string& name, native::XModel** arr, size_t count, native::XModel** original_arr,
                                 rapidjson::Value& container, rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator>& allocator) const;

        void write_sounds_array(const std::string& name, native::snd_alias_list_t** arr, size_t count, native::snd_alias_list_t** original_arr,
                                rapidjson::Value& container, rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator>& allocator) const;

        void read_weapon_anims(const std::string& name, const char*** arr, const char*** original, const rapidjson::Value& container) const;

		void read_sounds_array(const std::string& name, native::snd_alias_list_t*** destination, const rapidjson::Value& container) const;

		void read_xmodels_array(const std::string& name, native::XModel*** destination, const rapidjson::Value& container) const;

        void write_weapon_anims(const std::string& name, const char** arr, const char** original_arr, rapidjson::Value& container,
                                rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator>& allocator) const;

        bool write_variant(rapidjson::Value& parent_value, rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator>& allocator,
                           const native::XAssetHeader& weapon_original, const native::XAssetHeader& weapon_variant) const;

        bool read_variant(native::XAssetHeader& weapon, const rapidjson::Value& variant, const native::XAssetHeader& original) const;
    };

} // namespace iw4of::interfaces
