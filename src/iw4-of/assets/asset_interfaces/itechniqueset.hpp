#pragma once

#include "assets/asset_interface.hpp"

namespace iw4of::interfaces
{
	struct itechniqueset : asset_interface
	{
	public:
		itechniqueset(const iw4of::assets* assets) : asset_interface(assets) {}

	protected:
		bool write_internal(const native::XAssetHeader& header) const override;
		void* read_internal(const std::string& name) const override;

		std::filesystem::path get_file_name(const std::string& basename) const override;
		std::filesystem::path get_folder_name() const override
		{
			return "techsets";
		}


	private:

		void read_technique(native::MaterialTechnique** tech, const std::string& name) const;

		std::filesystem::path get_technique_work_path(const std::string& name) const;

		bool write(const native::MaterialTechnique* iw4_technique) const;
	};
}