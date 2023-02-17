#pragma once
#include <filesystem>
#include <functional>

#include "params.hpp"
#include "asset_interface.hpp"
#include <game/structs.hpp>

namespace iw4of
{
  class assets final
  {
#define STRINGIZE_(x) #x
#define STRINGIZE(x) STRINGIZE_(x)
#define RAPIDJSON_STR(str) (str == nullptr ? rapidjson::Value(rapidjson::kNullType) : static_cast<rapidjson::Value>(rapidjson::StringRef(str)))
#define AssertSize(x, size) static_assert(sizeof(x) == size, STRINGIZE(x) " structure has an invalid size.")

   public:
    template <typename T>
    bool write(int iw4_int_type, T* header) const
    {
      if (check_type_is_supported(iw4_int_type))
      {
        return asset_interfaces[iw4_int_type]->write<T>(header);
      }

      return false;
    }

    template <typename T>
    T* read(int iw4_int_type, const std::string& name) const
    {
      if (check_type_is_supported(iw4_int_type))
      {
        return asset_interfaces[iw4_int_type]->read<T>(name);
      }

      return nullptr;
    }

    void set_work_path(const std::filesystem::path& path)
    {
      params.work_directory = path;
    }

    std::string read_file(const std::string& name) const;

    std::string get_work_directory() const;

    void print_error(const std::string& message) const;

    void print(const std::string& message) const;

    void* find_other_asset(int type, const std::string& name) const;

    uint32_t write_in_stringtable(const std::string& text) const;

    std::string read_from_stringtable(const uint32_t& index) const;

    assets(const params_t& params);
    ~assets();

   private:
    bool interface_exists(native::XAssetType assetType) const;
    bool check_type_is_supported(int iw4_int_type) const;

    params_t params;

    asset_interface* asset_interfaces[native::ASSET_TYPE_COUNT]{};
  };
} // namespace iw4of