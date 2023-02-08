#include "std_include.hpp"
#include "asset_interface.hpp"
#include "assets.hpp"

void* iw4of::asset_interface::find_internal(iw4of::native::XAssetType type, const std::string& name) const
{
	return assets->find_other_asset(type, name);
}

void iw4of::asset_interface::print_internal(const std::string& message) const
{
	assets->print(message);
}

void iw4of::asset_interface::print_error_internal(const std::string& message) const
{
	assets->print_error(message);
}