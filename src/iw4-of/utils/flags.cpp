#include <std_include.hpp>
#include "flags.hpp"
#include "string.hpp"

namespace utils::flags
{
	void parse_flags(std::vector<std::string>& flags)
	{
		int num_args;
		const auto argv = CommandLineToArgvW(GetCommandLineW(), &num_args);

		flags.clear();

		if (argv)
		{
			for (auto i = 0; i < num_args; ++i)
			{
				std::wstring wide_flag(argv[i]);
				if (wide_flag[0] == L'-')
				{
					flags.emplace_back(wide_flag.begin() + 1, wide_flag.end());
				}
			}

			LocalFree(argv);
		}
	}

	bool has_flag(const std::string& flag)
	{
		static auto parsed = false;
		static std::vector<std::string> enabled_flags;

		if (!parsed)
		{
			parse_flags(enabled_flags);
			parsed = true;
		}

		for (const auto& entry : enabled_flags)
		{
			if (string::to_lower(entry) == string::to_lower(flag))
			{
				return true;
			}
		}

		return false;
	}
}
