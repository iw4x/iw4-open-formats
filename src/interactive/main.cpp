#include <string>
#include <vector>
#include <stdio.h>
#include <windows.h>
#include <iostream>

#include "utils/io.hpp"
#include "assets/assets.hpp"

void print(int a, const std::string& str)
{
	printf("%i %s\n", a, str.data());
}

int main() {
	// printf() displays the string inside quotation

	printf("enter work path to begin:\n");
	std::string work_path;
	std::getline(std::cin, work_path);


	while (true)
	{
		printf("enter asset type to test:\n");
		int type = 0;
		scanf_s("%i", &type);

		printf("enter asset name to test:\n");
		std::cin.ignore();
		std::string input_str;
		std::getline(std::cin, input_str);

		if (input_str == "exit")
		{
			return 0;
		}

		{
			const auto& params = iw4of::assets::params_t(
				std::filesystem::path(work_path),
				nullptr,
				nullptr,
				print
			);

			iw4of::assets iw4of(params);

			printf("Reading...\n");
			auto result = iw4of.read<void>(type, input_str);

			if (result)
			{
				printf("Writing...\n");
				bool success = iw4of.write<void>(type, result);

				if (success)
				{
					printf("Success!\n");
				}
				else {
					printf("Failed. You might need a debugger to know why.\n");
				}
			}
			else {
				printf("Failed reading, result is nullptr\n");
			}
		}

	}

	return 1;
}