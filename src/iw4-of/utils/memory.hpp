#pragma once

#include <mutex>

namespace iw4of::utils
{
    class memory final
    {
       public:
        class allocator final
        {
           public:
            ~allocator();

            void clear();

            void free(void* data);

            void free(const void* data);

            void* allocate(size_t length);

            template <typename T>
            T* allocate()
            {
                return this->allocate_array<T>(1);
            }

            template <typename T>
            T* allocate_array(const size_t count = 1)
            {
                return static_cast<T*>(this->allocate(count * sizeof(T)));
            }

            bool empty() const;

            char* duplicate_string(const std::string& string);

            bool is_pointer_mapped(void* ptr) const
            {
                return this->ptr_map.contains(ptr);
            }

            template <typename T>
            T* get_pointer(void* oldPtr)
            {
                if (this->is_pointer_mapped(oldPtr))
                {
                    return static_cast<T*>(this->ptr_map[oldPtr]);
                }

                return nullptr;
            }

            void map_pointer(void* oldPtr, void* newPtr)
            {
                this->ptr_map[oldPtr] = newPtr;
            }

           private:
            std::unordered_map<void*, void*> ptr_map;
            std::mutex mutex_;
            std::vector<void*> pool_;
        };

        static void* allocate(size_t length);

        template <typename T>
        static T* allocate()
        {
            return allocate_array<T>(1);
        }

        template <typename T>
        static T* allocate_array(const size_t count = 1)
        {
            return static_cast<T*>(allocate(count * sizeof(T)));
        }

        static char* duplicate_string(const std::string& string);

        static void free(void* data);
        static void free(const void* data);

        static bool is_set(const void* mem, char chr, size_t length);

        static allocator* get_allocator();

       private:
        static allocator mem_allocator_;
    };
} // namespace iw4of::utils
