#pragma once
#include <game/structs.hpp>
#include <utils/memory.hpp>
#include <string>

namespace iw4of
{
    class asset_interface
    {
#define RETURN_IF_NULL(x)                                                             \
    if (x == nullptr)                                                                 \
    {                                                                                 \
        print_error("{} was unexpectedly null! could not find the asset for it", #x); \
        assert(false);                                                                \
        return 0;                                                                     \
    }

       public:
        template <typename T>
        bool write(T* header) const
        {
            return write_internal({header});
        }

        template <typename T>
        T* read(const std::string name) const
        {
            return reinterpret_cast<T*>(read_internal(name));
        };

        asset_interface(const class assets* assets)
        {
            this->assets = assets;
        }

        std::filesystem::path get_work_path(const native::XAssetHeader& header) const;

        virtual ~asset_interface() {}

       protected:
        virtual void* read_internal(const std::string& name) const = 0;
        virtual bool write_internal(const native::XAssetHeader& header) const = 0;
        virtual std::filesystem::path get_file_name(const std::string& asset_name) const = 0;
        virtual std::filesystem::path get_folder_name() const = 0;
        virtual std::filesystem::path get_file_name(const native::XAssetHeader& header) const
        {
            return get_file_name(header.physPreset->name);
        };

        std::filesystem::path get_work_path(const std::string& asset_name) const;

        std::filesystem::path get_work_path(const std::filesystem::path& file_path) const;

        mutable utils::memory::allocator local_allocator{};

        template <typename T>
        T* find(iw4of::native::XAssetType type, const std::string& name) const
        {
            return reinterpret_cast<T*>(this->find_internal(type, name));
        }

        template <typename... Args>
        void print(const std::string_view& fmt, Args&&... args) const
        {
            this->print_internal(std::vformat(fmt, std::make_format_args(args...)));
        }

        template <typename... Args>
        void print_error(const std::string_view& fmt, Args&&... args) const
        {
            this->print_error_internal(std::vformat(fmt, std::make_format_args(args...)));
        }

        const class assets* assets;

       private:
        void* find_internal(iw4of::native::XAssetType type, const std::string& name) const;
        void print_error_internal(const std::string& message) const;
        void print_internal(const std::string& message) const;
    };
} // namespace iw4of