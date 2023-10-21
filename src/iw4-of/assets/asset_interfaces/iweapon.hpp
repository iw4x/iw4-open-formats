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

		bool write_variant(rapidjson::Value& parent_value, rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator>& allocator,
                           const native::XAssetHeader&  weapon_original, const native::XAssetHeader& weapon_variant) const;
    };

} // namespace iw4of::interfaces
