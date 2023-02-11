#pragma once

#include <filesystem>

#include "game/structs.hpp"

#include "../asset_interface.hpp"
#include "../assets.hpp"
#include "utils/stream.hpp"

namespace iw4of::interfaces
{
	struct ifx : asset_interface
	{

	public:
		ifx(const iw4of::assets* assets) : asset_interface(assets) {}

	protected:
		bool write_internal(const native::XAssetHeader& header) const override;
		void* read_internal(const std::string& name) const override;

		std::filesystem::path get_file_name(const std::string& basename) const override;
		std::filesystem::path get_folder_name() const override
		{
			return "fx";
		}

	private:
		void write(const native::FxElemVisuals* from, char elemType, utils::stream* buffer) const;
		void read(native::FxElemVisuals* into, char elemType, utils::stream::reader* reader) const;
	};
}