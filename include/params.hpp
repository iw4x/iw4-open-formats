#pragma once

#include <functional>
#include <filesystem>

namespace iw4of
{
    struct params_t
    {
        enum print_type
        {
            P_WARN,
            P_ERR
        };

        std::filesystem::path work_directory{};
        std::function<void*(int type, const std::string& name)> find_other_asset{};
        std::function<void(print_type level, const std::string& message)> print{};
        std::function<std::string(const std::string& filename)> fs_read_file{};
        std::function<unsigned int(const std::string& text)> store_in_string_table{};
        std::function<std::string(const unsigned int index)> get_from_string_table{};

        bool write_only_once = false;

        params_t(const std::filesystem::path& work_directory)
        {
            this->work_directory = work_directory;
        }

        params_t(){};
    };
} // namespace iw4of
