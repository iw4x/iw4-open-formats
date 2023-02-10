#pragma once
#include <filesystem>
#include <functional>

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
		struct params_t
		{
			enum print_type
			{
				P_WARN, P_ERR
			};

			std::filesystem::path work_directory{};
			std::function<void*(int type, const std::string& name)> find_other_asset{};
			std::function<void(print_type, const std::string&)> print{};
			std::function<std::string(const std::string& filename)> fs_read_file{};

			params_t(
				const std::filesystem::path& work_directory,
				std::function<void* (int type, const std::string& name)> find_other_asset = nullptr,
				std::function<std::string(const std::string& filename)> fs_read_file = nullptr,
				std::function<void(print_type, const std::string&)> print_function = nullptr
			);

			params_t() {};
		};

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
		
		std::string read_file(const std::string& name) const;

		std::string get_work_directory() const;

		void print_error(const std::string& message) const;

		void print(const std::string& message) const;

		void* find_other_asset(int type, const std::string& name) const;
		
		assets(const params_t& params);

	private:
		bool interface_exists(native::XAssetType assetType) const;
		bool check_type_is_supported(int iw4_int_type) const;

		params_t params;

		asset_interface* asset_interfaces[native::ASSET_TYPE_COUNT]{};
	};
}