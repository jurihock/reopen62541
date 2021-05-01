#pragma once

#include <open62541.h>

#include <codecvt>
#include <string>
#include <typeinfo>
#include <vector>

namespace ua
{
  namespace convert
  {
    template<typename T>
    inline static std::string to_string()
    {
      return typeid(T).name();
    }

    inline static std::string to_string(const std::wstring& value)
    {
      // TODO: deprecated in C++17 
      std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> convert;

      return convert.to_bytes(value);
    }

    inline static std::string to_string(const char* value, const size_t size)
    {
      return std::string(value, size);
    }

    inline static std::wstring to_wstring(const std::string& value)
    {
      return std::wstring(value.begin(), value.end());
    }

    inline static std::wstring to_wstring(const char* value, const size_t size)
    {
      // TODO: deprecated in C++17
      std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> convert;

      return convert.from_bytes(value, value + size);
    }

    inline static std::string to_string(const UA_String& value)
    {
      return ua::convert::to_string(reinterpret_cast<char*>(value.data), value.length);
    }

    inline static std::string to_string(const UA_String* value)
    {
      return ua::convert::to_string(reinterpret_cast<char*>(value->data), value->length);
    }

    inline static std::wstring to_wstring(const UA_String& value)
    {
      return ua::convert::to_wstring(reinterpret_cast<char*>(value.data), value.length);
    }

    inline static std::wstring to_wstring(const UA_String* value)
    {
      return ua::convert::to_wstring(reinterpret_cast<char*>(value->data), value->length);
    }

    template<typename T>
    static uint32_t to_ua_data_type()
    {
      // SCALARS

      if constexpr (std::is_same<T, bool>::value)
        return UA_TYPES_BOOLEAN;

      if constexpr (std::is_same<T, int8_t>::value)
        return UA_TYPES_SBYTE;

      if constexpr (std::is_same<T, uint8_t>::value)
        return UA_TYPES_BYTE;

      if constexpr (std::is_same<T, int16_t>::value)
        return UA_TYPES_INT16;

      if constexpr (std::is_same<T, uint16_t>::value)
        return UA_TYPES_UINT16;

      if constexpr (std::is_same<T, int32_t>::value)
        return UA_TYPES_INT32;

      if constexpr (std::is_same<T, uint32_t>::value)
        return UA_TYPES_UINT32;

      if constexpr (std::is_same<T, int64_t>::value)
        return UA_TYPES_INT64;

      if constexpr (std::is_same<T, uint64_t>::value)
        return UA_TYPES_UINT64;

      if constexpr (std::is_same<T, float>::value)
        return UA_TYPES_FLOAT;

      if constexpr (std::is_same<T, double>::value)
        return UA_TYPES_DOUBLE;

      if constexpr (std::is_same<T, std::string>::value)
        return UA_TYPES_STRING;

      // VECTORS

      if constexpr (std::is_same<T, std::vector<bool>>::value)
        return UA_TYPES_BOOLEAN;

      if constexpr (std::is_same<T, std::vector<int8_t>>::value)
        return UA_TYPES_SBYTE;

      if constexpr (std::is_same<T, std::vector<uint8_t>>::value)
        return UA_TYPES_BYTE;

      if constexpr (std::is_same<T, std::vector<int16_t>>::value)
        return UA_TYPES_INT16;

      if constexpr (std::is_same<T, std::vector<uint16_t>>::value)
        return UA_TYPES_UINT16;

      if constexpr (std::is_same<T, std::vector<int32_t>>::value)
        return UA_TYPES_INT32;

      if constexpr (std::is_same<T, std::vector<uint32_t>>::value)
        return UA_TYPES_UINT32;

      if constexpr (std::is_same<T, std::vector<int64_t>>::value)
        return UA_TYPES_INT64;

      if constexpr (std::is_same<T, std::vector<uint64_t>>::value)
        return UA_TYPES_UINT64;

      if constexpr (std::is_same<T, std::vector<float>>::value)
        return UA_TYPES_FLOAT;

      if constexpr (std::is_same<T, std::vector<double>>::value)
        return UA_TYPES_DOUBLE;

      if constexpr (std::is_same<T, std::vector<std::string>>::value)
        return UA_TYPES_STRING;

      // ELSE

      throw std::invalid_argument("Invalid data type!");
    }

    template<typename T> struct is_scalar : public std::true_type {};
    template<typename T, typename _> struct is_scalar<std::vector<T, _>> : public std::false_type {};

    template<typename T>
    static int32_t to_ua_value_rank()
    {
      return ua::convert::is_scalar<T>()
        ? UA_VALUERANK_SCALAR
        : UA_VALUERANK_SCALAR_OR_ONE_DIMENSION;
    }
  }
}
