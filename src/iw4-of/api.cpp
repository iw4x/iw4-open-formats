#include "std_include.hpp"

#include "assets/assets.hpp"
#include <api.hpp>

iw4of::api::~api()
{
    delete (_assets);
}

iw4of::api::api(const params_t& params)
{
    _assets = new assets(params);
}

bool iw4of::api::write_internal(int t, void* asset) const
{
    return _assets->write<void>(t, asset);
}

void* iw4of::api::read_internal(int t, const std::string& name) const
{
    return _assets->read<void>(t, name);
}

void iw4of::api::set_work_path(const std::filesystem::path& path)
{
    _assets->set_work_path(path);
}

std::unordered_set<iw4of::iw4_native_asset, iw4of::iw4_native_asset::hash> iw4of::api::get_children(int type, void* asset) const
{
	return _assets->get_children(type, asset);
}

void iw4of::api::clear_writes() const
{
    _assets->clear_writes();
}

bool iw4of::api::is_type_supported(int iw4_type) const
{
    return _assets->is_type_supported(iw4_type);
}
