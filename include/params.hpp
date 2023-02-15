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
    std::function<unsigned int(const std::string& text)>
        store_in_string_table{};
    std::function<std::string(const unsigned int& index)>
        get_from_string_table{};

    params_t(const std::filesystem::path& work_directory,

             // All of these are somewhat facultative but if you want solid data
             // you'll want all of them
             std::function<void(print_type level, const std::string& message)>
                 print_function = nullptr,
             std::function<void*(int type, const std::string& name)>
                 find_other_asset = nullptr,
             std::function<std::string(const std::string& filename)>
                 fs_read_file = nullptr,
             std::function<unsigned int(const std::string& text)>
                 store_in_string_table = nullptr,
             std::function<std::string(const unsigned int& index)>
                 get_from_string_table = nullptr)
    {
      this->find_other_asset = find_other_asset;
      this->work_directory = work_directory;
      this->print = print_function;
      this->fs_read_file = fs_read_file;
      this->store_in_string_table = store_in_string_table;
      this->get_from_string_table = get_from_string_table;
    }

    params_t(){};
  };
} // namespace iw4of