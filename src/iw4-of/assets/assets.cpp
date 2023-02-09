#include <std_include.hpp>

//#include <module/asset_interfaces/ixmodel.hpp>
//#include <module/asset_interfaces/iphyspreset.hpp>
//#include <module/asset_interfaces/igfxworld.hpp>
//#include <module/asset_interfaces/iglassworld.hpp>
//#include <module/asset_interfaces/icomworld.hpp>
//#include <module/asset_interfaces/ilightdef.hpp>
//#include <module/asset_interfaces/irawfile.hpp>
//#include <module/asset_interfaces/imapents.hpp>
//#include <module/asset_interfaces/isndalias.hpp>
//#include <module/asset_interfaces/isndcurve.hpp>
//#include <module/asset_interfaces/iloadedsound.hpp>
//#include <module/asset_interfaces/ifxworld.hpp>
//#include <module/asset_interfaces/ifx.hpp>
//#include <module/asset_interfaces/ixanimparts.hpp>

#include <assets/asset_interfaces/iclipmap.hpp>
#include <assets/asset_interfaces/igfximage.hpp>
#include <assets/asset_interfaces/imaterial.hpp>
#include <assets/asset_interfaces/itechniqueset.hpp>
#include <assets/asset_interfaces/ipixelshader.hpp>
#include <assets/asset_interfaces/ivertexshader.hpp>
#include <assets/asset_interfaces/ivertexdecl.hpp>

#include "assets.hpp"
#include <utils/io.hpp>
#include <utils/flags.hpp>

namespace iw4of
{
	bool assets::interface_exists(native::XAssetType assetType) const
	{
		return asset_interfaces[assetType] != nullptr;
	}

	bool assets::check_type_is_supported(int iw4_int_type) const
	{
		if (iw4_int_type < 0 || iw4_int_type >= native::ASSET_TYPE_COUNT)
		{
			print_error(std::format("invalid asset type {}", iw4_int_type));
			return false;
		}

		auto iw4of_type = static_cast<iw4of::native::XAssetType>(iw4_int_type);

		if (!interface_exists(iw4of_type))
		{
			print_error(std::format("no interface for asset type {}", iw4_int_type));
			return false;
		}

		return true;
	}

	std::string assets::read_file(const std::string& name) const
	{
		if (params.fs_read_file)
		{
			return params.fs_read_file(name);
		}

		// Fallback
		return utils::io::read_file(name);
	}

	std::string assets::get_work_directory() const
	{
		return params.work_directory.string();
	}

	void assets::print_error(const std::string& message) const
	{
		if (params.print)
		{
			params.print(params_t::print_type::P_ERR, message);
		}
	}

	void assets::print(const std::string& message) const
	{
		if (params.print)
		{
			params.print(params_t::print_type::P_WARN, message);
		}
	}

	void* assets::find_other_asset(int type, const std::string& name) const
	{
		if (params.find_other_asset)
		{
			return params.find_other_asset(type, name);
		}

		return nullptr;
	}

	assets::assets(const params_t& params)
	{
		this->params = params;

		asset_interfaces[native::XAssetType::ASSET_TYPE_IMAGE] = new interfaces::igfximage(this);
		asset_interfaces[native::XAssetType::ASSET_TYPE_MATERIAL] = new interfaces::imaterial(this);
		asset_interfaces[native::XAssetType::ASSET_TYPE_TECHNIQUE_SET] = new interfaces::itechniqueset(this);
		asset_interfaces[native::XAssetType::ASSET_TYPE_PIXELSHADER] = new interfaces::ipixelshader(this);
		asset_interfaces[native::XAssetType::ASSET_TYPE_VERTEXSHADER] = new interfaces::ivertexshader(this);
		asset_interfaces[native::XAssetType::ASSET_TYPE_VERTEXDECL] = new interfaces::ivertexdecl(this);
		//asset_interfaces[native::XAssetType::ASSET_TYPE_PHYSPRESET] = new interfaces::iphyspreset();
		//asset_interfaces[native::XAssetType::ASSET_TYPE_XMODEL] = new interfaces::ixmodel();
		//asset_interfaces[native::XAssetType::ASSET_TYPE_GFXWORLD] = new interfaces::igfxworld();
		//asset_interfaces[native::XAssetType::ASSET_TYPE_GAMEWORLD_MP] = new interfaces::iglassworld();
		//asset_interfaces[native::XAssetType::ASSET_TYPE_COMWORLD] = new interfaces::icomworld();
		//asset_interfaces[native::XAssetType::ASSET_TYPE_LIGHT_DEF] = new interfaces::ilightdef();
		//asset_interfaces[native::XAssetType::ASSET_TYPE_RAWFILE] = new interfaces::irawfile();
		//asset_interfaces[native::XAssetType::ASSET_TYPE_MAP_ENTS] = new interfaces::imapents();
		asset_interfaces[native::XAssetType::ASSET_TYPE_CLIPMAP_MP] = new interfaces::iclipmap(this);
		//asset_interfaces[native::XAssetType::ASSET_TYPE_SOUND] = new interfaces::isndalias();
		//asset_interfaces[native::XAssetType::ASSET_TYPE_LOADED_SOUND] = new interfaces::iloadedsound();
		//asset_interfaces[native::XAssetType::ASSET_TYPE_SOUND_CURVE] = new interfaces::isndcurve();
		//asset_interfaces[native::XAssetType::ASSET_TYPE_FXWORLD] = new interfaces::ifxworld();
		//asset_interfaces[native::XAssetType::ASSET_TYPE_FX] = new interfaces::ifx();
		//asset_interfaces[native::XAssetType::ASSET_TYPE_XANIMPARTS] = new interfaces::ixanimparts();
	}

	assets::params_t::params_t(const std::filesystem::path& work_directory, std::function<void* (int type, const std::string& name)> find_other_asset, std::function<std::string(const std::string& filename)> fs_read_file, std::function<void* (int, const std::string&)> print_function)
	{
		this->find_other_asset = find_other_asset;
		this->work_directory = work_directory;
		this->print = print_function;
		this->fs_read_file = fs_read_file;
	}

}