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

			params_t(
				std::filesystem::path work_directory,
				std::function<void* (int type, const std::string& name)> find_other_asset = nullptr,
				std::function<void* (int, const std::string&)> print_function = nullptr
			)
			{
				this->find_other_asset = find_other_asset;
				this->work_directory = work_directory;
				this->print = print_function;
			};

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
		T* read(int iw4_int_type, const std::string& name)
		{
			if (check_type_is_supported(iw4_int_type))
			{
				return asset_interfaces[iw4_int_type]->read<T>(name);
			}

			return nullptr;
		}
		

		std::string get_work_directory() const
		{
			return params.work_directory.string();
		}

		void print_error(const std::string& message) const
		{
			if (params.print)
			{
				params.print(params_t::print_type::P_ERR, message);
			}
		}

		void print(const std::string& message) const
		{
			if (params.print)
			{
				params.print(params_t::print_type::P_WARN, message);
			}
		}

		void* find_other_asset(int type, const std::string& name) const
		{
			if (params.find_other_asset)
			{
				return params.find_other_asset(type, name);
			}

			return nullptr;
		}
		
		assets(const params_t& params);

	private:
		bool interface_exists(native::XAssetType assetType) const;
		bool check_type_is_supported(int iw4_int_type) const
		{
			if (iw4_int_type < 0 || iw4_int_type >= native::ASSET_TYPE_COUNT)
			{
				print_error(std::format("invalid asset type {}", iw4_int_type));
				return false;
			}

			auto iw4of_type = static_cast<iw4of::native::XAssetType>(iw4_int_type);

			if (!interface_exists(iw4of_type))
			{
				print_error(std::format("no interface for asset type {}", iw4_int_type));
				return false;
			}

			return true;
		}

		params_t params;

		class asset_interface* asset_interfaces[native::ASSET_TYPE_COUNT]{};
	};
}