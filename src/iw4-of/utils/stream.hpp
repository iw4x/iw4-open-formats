#pragma once

#define POINTER 255
#define FOLLOWING 254

#include <string>

namespace iw4of::utils
{
    class stream
    {
       private:
        std::string buffer;
        std::unordered_map<void*, size_t> data_pointers;

       public:
        class reader
        {
           public:
            reader(utils::memory::allocator* allocator, std::string& buffer)
                : position_(0)
                , buffer_(std::move(buffer))
                , allocator_(allocator)
            {
            }

            std::string read_string();
            const char* read_cstring();

            char read_byte();

            void* read(std::size_t size, std::size_t count = 1);
            template <typename T>
            T* read_object()
            {
                return read_array<T>(1);
            }

            template <typename T>
            T* read_array_once(std::size_t count = 1)
            {
                auto b = static_cast<uint8_t>(read_byte());
                switch (b)
                {
                    case POINTER:
                    {
                        auto ptr = read<int32_t>();
                        auto* voidPtr = reinterpret_cast<void*>(ptr);

                        if (this->allocator_->is_pointer_mapped(voidPtr))
                        {
                            return this->allocator_->get_pointer<T>(voidPtr);
                        }

                        throw std::runtime_error("Bad data: missing ptr");
                    }
                    case FOLLOWING:
                    {
                        auto filePosition = this->position_;
                        auto data = read_array<T>(count);
                        this->allocator_->map_pointer(reinterpret_cast<void*>(filePosition), data);
                        return data;
                    }
                    default: throw std::runtime_error("Bad data");
                }
            }

            template <typename T>
            T* read_array(std::size_t count = 1)
            {
                return static_cast<T*>(this->read(sizeof(T), count));
            }

            template <typename T>
            T read()
            {
                T obj;

                for (uint32_t i = 0; i < sizeof(T); ++i)
                {
                    reinterpret_cast<char*>(&obj)[i] = this->read_byte();
                }

                return obj;
            }

            bool end() const;
            void seek(uint32_t position);
            void seek_relative(uint32_t position);
            void map_pointer(void* oldPointer, void* newPointer);
            bool has_pointer(void* pointer) const;

           private:
            uint32_t position_;
            std::string buffer_;
            std::map<void*, void*> pointer_map;
            utils::memory::allocator* allocator_;
        };

        stream();
        stream(size_t size);
        ~stream();

        class offset
        {
           public:
            offset(stream* _stream, size_t _offset)
                : _offset(_offset)
                , stream(_stream)
            {
            }

            template <typename T>
            T* as()
            {
                return reinterpret_cast<T*>(this->stream->data() + _offset);
            }

           private:
            size_t _offset;
            stream* stream;
        };

        size_t length();
        size_t capacity();

        char* save(const void* _str, size_t size, size_t count = 1);
        char* save(int value, size_t count = 1);
        template <typename T>
        inline char* save_object(T value)
        {
            return save_array(&value, 1);
        }
        template <typename T>
        inline char* save_array(T* array, size_t count)
        {
            return save(array, sizeof(T), count);
        }
        template <typename T>
        inline void save_array_if_not_existing(T* data, size_t count)
        {
            if (data_pointers.contains(data))
            {
                size_t filePosition = data_pointers.at(data);
                save_byte(POINTER);
                save_object(filePosition);
            }
            else
            {
                save_byte(FOLLOWING);
                data_pointers.insert_or_assign(reinterpret_cast<void*>(data), length());
                save_array(data, count);
            }
        }

        char* save_string(std::string string);
        char* save_string(const char* string);
        char* save_string(const char* string, size_t len);
        char* save_byte(uint8_t byte, size_t count = 1);
        char* save_null(size_t count = 1);

        char* save_text(std::string string);

        offset _offset();
        char* data();
        char* at();
        template <typename T>
        T* Dest()
        {
            return reinterpret_cast<T*>(this->at());
        }

        void to_buffer(std::string& outBuffer);
        std::string to_buffer();
    };
} // namespace iw4of::utils
