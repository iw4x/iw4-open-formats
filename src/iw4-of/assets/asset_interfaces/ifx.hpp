#pragma once

#include <filesystem>

#include <game/structs.hpp>

#include "../asset_interface.hpp"
#include "../assets.hpp"

#include "rapidjson/document.h"

#include <utils/stream.hpp>

namespace iw4of::interfaces
{
    struct ifx : asset_interface
    {
       public:
        ifx(const iw4of::assets* assets)
            : asset_interface(assets)
        {
        }

       protected:
        bool write_internal(const native::XAssetHeader& header) const override;
        void* read_internal(const std::string& name) const override;

        std::filesystem::path get_file_name(const std::string& basename) const override;
        std::filesystem::path get_folder_name() const override
        {
            return "fx";
        }

       private:
        rapidjson::Value to_json(const native::FxElemVisuals* visuals, char elemType,
                                 rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator>& allocator) const;
		void read(native::FxElemVisuals* visuals, char elemType, utils::stream::reader* reader) const;
		void read(native::FxElemVisuals* into, char elemType, const rapidjson::Value& value) const;

		std::filesystem::path get_legacy_work_path(const std::string& file_name) const;
        std::filesystem::path get_binary_file_name(const std::string& basename) const;

        native::FxEffectDef* read_binary(const std::string& name, const std::string& path) const;
        native::FxEffectDef* read_plaintext(const std::string& name, const std::string& path) const;
	};
} // namespace iw4of::interfaces
