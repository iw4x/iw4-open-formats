#include <std_include.hpp>

#include <assets/asset_interfaces/iphyspreset.hpp>
#include <assets/asset_interfaces/iglassworld.hpp>
#include <assets/asset_interfaces/icomworld.hpp>
#include <assets/asset_interfaces/ilightdef.hpp>
#include <assets/asset_interfaces/irawfile.hpp>
#include <assets/asset_interfaces/ifxworld.hpp>
#include <assets/asset_interfaces/ifx.hpp>
#include <assets/asset_interfaces/ixanimparts.hpp>
#include <assets/asset_interfaces/iclipmap.hpp>
#include <assets/asset_interfaces/igfximage.hpp>
#include <assets/asset_interfaces/imaterial.hpp>
#include <assets/asset_interfaces/itechniqueset.hpp>
#include <assets/asset_interfaces/ipixelshader.hpp>
#include <assets/asset_interfaces/ivertexshader.hpp>
#include <assets/asset_interfaces/ivertexdecl.hpp>
#include <assets/asset_interfaces/isndalias.hpp>
#include <assets/asset_interfaces/isndcurve.hpp>
#include <assets/asset_interfaces/iloadedsound.hpp>
#include <assets/asset_interfaces/igfxworld.hpp>
#include <assets/asset_interfaces/imapents.hpp>
#include <assets/asset_interfaces/ixmodel.hpp>
#include <assets/asset_interfaces/iphyscollmap.hpp>
#include <assets/asset_interfaces/iweapon.hpp>
#include <assets/asset_interfaces/itracerdef.hpp>


#include "assets.hpp"
#include <utils/io.hpp>

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

    uint32_t assets::write_in_stringtable(const std::string& text) const
    {
        if (params.store_in_string_table)
        {
            return params.store_in_string_table(text);
        }

        print_error(std::format("IW4OF is missing stringtable function and cannot store text {} from stringtable!", text));
        return 0xDEADC0DE;
    }

    std::string assets::read_from_stringtable(const uint32_t& index) const
    {
        if (params.get_from_string_table)
        {
            return params.get_from_string_table(index);
        }

        print_error(std::format("IW4OF is missing stringtable function and cannot read index {} from stringtable!", index));
        return std::format("iw4of-missing-{}", index);
    }

    bool assets::can_write(const std::filesystem::path& asset_predicted_path) const
    {
        return !params.write_only_once || !written_assets.contains(asset_predicted_path.string());
    }

    void assets::clear_writes()
    {
        if (!params.write_only_once)
        {
            print_error("You do not need to clear writes if write_once is not enabled.");
        }

        written_assets.clear();
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
        asset_interfaces[native::XAssetType::ASSET_TYPE_PHYSPRESET] = new interfaces::iphyspreset(this);
        asset_interfaces[native::XAssetType::ASSET_TYPE_XMODEL] = new interfaces::ixmodel(this);
        asset_interfaces[native::XAssetType::ASSET_TYPE_GFXWORLD] = new interfaces::igfxworld(this);
        asset_interfaces[native::XAssetType::ASSET_TYPE_GAMEWORLD_MP] = new interfaces::iglassworld(this);
        asset_interfaces[native::XAssetType::ASSET_TYPE_COMWORLD] = new interfaces::icomworld(this);
        asset_interfaces[native::XAssetType::ASSET_TYPE_LIGHT_DEF] = new interfaces::ilightdef(this);
        asset_interfaces[native::XAssetType::ASSET_TYPE_RAWFILE] = new interfaces::irawfile(this);
        asset_interfaces[native::XAssetType::ASSET_TYPE_MAP_ENTS] = new interfaces::imapents(this);
        asset_interfaces[native::XAssetType::ASSET_TYPE_CLIPMAP_MP] = new interfaces::iclipmap(this);
        asset_interfaces[native::XAssetType::ASSET_TYPE_SOUND] = new interfaces::isndalias(this);
        asset_interfaces[native::XAssetType::ASSET_TYPE_LOADED_SOUND] = new interfaces::iloadedsound(this);
        asset_interfaces[native::XAssetType::ASSET_TYPE_SOUND_CURVE] = new interfaces::isndcurve(this);
        asset_interfaces[native::XAssetType::ASSET_TYPE_FXWORLD] = new interfaces::ifxworld(this);
        asset_interfaces[native::XAssetType::ASSET_TYPE_FX] = new interfaces::ifx(this);
        asset_interfaces[native::XAssetType::ASSET_TYPE_XANIMPARTS] = new interfaces::ixanimparts(this);
        asset_interfaces[native::XAssetType::ASSET_TYPE_WEAPON] = new interfaces::iweapon(this);
        asset_interfaces[native::XAssetType::ASSET_TYPE_TRACER] = new interfaces::itracerdef(this);
    }

    assets::~assets()
    {
        for (size_t i = 0; i < native::XAssetType::ASSET_TYPE_COUNT; i++)
        {
            if (asset_interfaces[i])
            {
                delete asset_interfaces[i];
            }
        }
    }
} // namespace iw4of
