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

std::filesystem::path iw4of::asset_interface::get_work_path(const std::filesystem::path& asset_path) const
{
    std::stringstream final_path;

    final_path << assets->get_work_directory() << "/";

    if (!get_folder_name().empty())
    {
        final_path << get_folder_name().string() << "/";
    }

    final_path << asset_path.string();

    return final_path.str();
}

std::filesystem::path iw4of::asset_interface::get_work_path(const std::string& file_name) const
{
    return get_work_path(get_file_name(file_name));
}

std::filesystem::path iw4of::asset_interface::get_work_path(const native::XAssetHeader& header) const
{
    return get_work_path(get_file_name(header));
}
