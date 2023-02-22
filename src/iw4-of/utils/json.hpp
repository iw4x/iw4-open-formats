#pragma once

#include <rapidjson/document.h>
#include <game/structs.hpp>

namespace iw4of::utils::json
{
    // Helpers for rapidjson
    template <typename ValueType>
    struct rapidjson::internal::TypeHelper<ValueType, uint16_t>
    {
        static uint16_t Get(const ValueType& v)
        {
            return static_cast<uint16_t>(v.GetInt());
        }
    };

    template <typename ValueType>
    struct rapidjson::internal::TypeHelper<ValueType, short>
    {
        static short Get(const ValueType& v)
        {
            return static_cast<int16_t>(v.GetInt());
        }
    };

    template <typename ValueType>
    struct rapidjson::internal::TypeHelper<ValueType, std::string>
    {
        static std::string Get(const ValueType& v)
        {
            return v.GetString();
        }
    };

    template <typename ValueType>
    struct rapidjson::internal::TypeHelper<ValueType, char>
    {
        static char Get(const ValueType& v)
        {
            return static_cast<char>(v.GetInt());
        }
    };

    template <typename ValueType>
    struct rapidjson::internal::TypeHelper<ValueType, uint8_t>
    {
        static uint8_t Get(const ValueType& v)
        {
            return static_cast<uint8_t>(v.GetInt());
        }
    };

    template <class T>
    rapidjson::Value make_json_array(T* arr, size_t length, rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator>& allocator)
    {
        rapidjson::Value json_arr(rapidjson::kArrayType);

        for (size_t i = 0; i < length; i++)
        {
            rapidjson::Value value(arr[i]);
            json_arr.PushBack(value, allocator);
        }

        return json_arr;
    }

    iw4of::native::Bounds read_bounds(const rapidjson::Value& value);
    unsigned long read_flags(const std::string binaryFlags, std::size_t size);

    template <typename T>
    void copy_array(T* destination, const rapidjson::Value& json_member, size_t count = 0)
    {
        if (count == 0)
        {
            count = json_member.Size();
        }

        for (size_t i = 0; i < count; i++)
        {
            destination[i] = json_member[i].Get<T>();
        }
    }
} // namespace iw4of::utils::json
