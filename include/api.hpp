#pragma once

#include "params.hpp"
#include <functional>
#include <filesystem>

namespace iw4of
{
	class api final
	{

	public:
		template <typename T>
		bool write(int iw4_int_type, T* header) const
		{
			return write(iw4_int_type, header);
		}

		template <typename T>
		T* read(int iw4_int_type, const std::string& name) const
		{
			return reinterpret_cast<T*>(read(iw4_int_type, name));
		}

		api(const params_t& params);

		~api();

	private:

		class assets* _assets;

		bool write(int t, void* asset) const;

		void* read(int t, std::string name) const;

	};
}