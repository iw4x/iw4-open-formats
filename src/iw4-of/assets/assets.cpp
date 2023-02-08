#include <std_include.hpp>

//#include <module/asset_interfaces/igfximage.hpp>
//#include <module/asset_interfaces/imaterial.hpp>
//#include <module/asset_interfaces/itechniqueset.hpp>
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

#include "assets.hpp"
#include <utils/io.hpp>
#include <utils/flags.hpp>

namespace iw4of
{
	bool assets::interface_exists(native::XAssetType assetType) const
	{
		return asset_interfaces[assetType] != nullptr;
	}

	assets::assets(const params_t& params)
	{
		this->params = params;

		//asset_interfaces[native::XAssetType::ASSET_TYPE_IMAGE] = new interfaces::igfximage();
		//asset_interfaces[native::XAssetType::ASSET_TYPE_MATERIAL] = new interfaces::imaterial();
		//asset_interfaces[native::XAssetType::ASSET_TYPE_TECHNIQUE_SET] = new interfaces::itechniqueset();
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

}