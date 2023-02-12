#include "std_include.hpp"

#include "assets/assets.hpp"
#include "api.hpp"

iw4of::api::~api()
{
	delete(_assets);
}

iw4of::api::api(const params_t& params)
{
	_assets = new assets(params);
}

bool iw4of::api::write_internal(int t, void* asset) const
{
	return _assets->write<void>(t, asset);
}

void* iw4of::api::read_internal(int t, std::string name) const
{
	return _assets->read<void>(t, name);
}

void iw4of::api::set_work_path(const std::filesystem::path& path)
{
	_assets->set_work_path(path);
}