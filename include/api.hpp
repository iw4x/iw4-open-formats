#pragma once

#include "params.hpp"
#include "iw4_native_asset.hpp"
#include <functional>
#include <filesystem>
#include <unordered_set>

namespace iw4of
{
    class api final
    {
       public:

        bool write(int iw4_int_type, void* header) const
        {
            return write_internal(iw4_int_type, header);
        }

        template <typename T>
        T* read(int iw4_int_type, const std::string& name) const
        {
            return reinterpret_cast<T*>(read_internal(iw4_int_type, name));
        }

        void clear_writes() const;

        void set_work_path(const std::filesystem::path& path);

        bool is_type_supported(int iw4_type) const;

		std::unordered_set<iw4_native_asset, iw4_native_asset::hash> get_children(int type, void* asset) const;

        api(const params_t& params);

        ~api();

       private:
        class assets* _assets;

        bool write_internal(int t, void* asset) const;

        void* read_internal(int t, const std::string& name) const;
    };
} // namespace iw4of
