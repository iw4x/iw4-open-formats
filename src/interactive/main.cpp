#include <stdio.h>
#include <windows.h>
#include "assets/assets.hpp"

int main() {
	// printf() displays the string inside quotation

	const auto test_name = "my_clipmap";
	iw4of::native::clipMap_t clip {};

	clip.name = test_name;

	auto params = iw4of::assets::params_t(
		std::filesystem::path(".\\")
	);

	iw4of::assets iw4of(params);

	iw4of.write(iw4of::native::ASSET_TYPE_CLIPMAP_MP, &clip);

	auto str = std::string(test_name);
	auto returned_clip = iw4of.read<iw4of::native::clipMap_t>(iw4of::native::ASSET_TYPE_CLIPMAP_MP, str);

	return 0;
}