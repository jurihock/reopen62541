#pragma once

#include <string>
#include <vector>

#include <open62541.h>

namespace ua
{
  struct output
  {
    UA_Variant* variant;
    const size_t size;

    output(UA_Variant* variant, size_t size) : variant(variant), size(size) {}

    output operator[](const size_t index) const { return output(variant + index, 1); }

    template<typename T> void operator=(const T& value) { set<T>(value); }
    template<typename T> void operator<<(const T& value) { set<T>(value); }

    template<typename T> void set(const T& value)
    {
      // SCALARS

      if constexpr (std::is_same<T, bool>::value)
        UA_Variant_setScalarCopy(variant, &value, &UA_TYPES[UA_TYPES_BOOLEAN]);

      else if constexpr (std::is_same<T, int8_t>::value)
        UA_Variant_setScalarCopy(variant, &value, &UA_TYPES[UA_TYPES_SBYTE]);

      else if constexpr (std::is_same<T, uint8_t>::value)
        UA_Variant_setScalarCopy(variant, &value, &UA_TYPES[UA_TYPES_BYTE]);

      else if constexpr (std::is_same<T, int16_t>::value)
        UA_Variant_setScalarCopy(variant, &value, &UA_TYPES[UA_TYPES_INT16]);

      else if constexpr (std::is_same<T, uint16_t>::value)
        UA_Variant_setScalarCopy(variant, &value, &UA_TYPES[UA_TYPES_UINT16]);

      else if constexpr (std::is_same<T, int32_t>::value)
        UA_Variant_setScalarCopy(variant, &value, &UA_TYPES[UA_TYPES_INT32]);

      else if constexpr (std::is_same<T, uint32_t>::value)
        UA_Variant_setScalarCopy(variant, &value, &UA_TYPES[UA_TYPES_UINT32]);

      else if constexpr (std::is_same<T, int64_t>::value)
        UA_Variant_setScalarCopy(variant, &value, &UA_TYPES[UA_TYPES_INT64]);

      else if constexpr (std::is_same<T, uint64_t>::value)
        UA_Variant_setScalarCopy(variant, &value, &UA_TYPES[UA_TYPES_UINT64]);

      else if constexpr (std::is_same<T, float>::value)
        UA_Variant_setScalarCopy(variant, &value, &UA_TYPES[UA_TYPES_FLOAT]);

      else if constexpr (std::is_same<T, double>::value)
        UA_Variant_setScalarCopy(variant, &value, &UA_TYPES[UA_TYPES_DOUBLE]);

      else if constexpr (std::is_same<T, std::string>::value)
      {
        UA_String copy = UA_String_fromChars(value.c_str());
        UA_Variant_setScalarCopy(variant, &copy, &UA_TYPES[UA_TYPES_STRING]);
        UA_String_deleteMembers(&copy);
      }

      // TODO std::wstring

      //else if constexpr (std::is_same<T, std::wstring>::value)
      //{
      //  UA_String copy = UA_String_fromChars(value.c_str());
      //  UA_Variant_setScalarCopy(variant, &copy, &UA_TYPES[UA_TYPES_STRING]);
      //  UA_String_deleteMembers(&copy);
      //}

      // VECTORS

      else if constexpr (std::is_same<T, std::vector<int8_t>>::value)
        UA_Variant_setArrayCopy(variant, value.data(), value.size(), &UA_TYPES[UA_TYPES_SBYTE]);

      else if constexpr (std::is_same<T, std::vector<uint8_t>>::value)
        UA_Variant_setArrayCopy(variant, value.data(), value.size(), &UA_TYPES[UA_TYPES_BYTE]);

      else if constexpr (std::is_same<T, std::vector<int16_t>>::value)
        UA_Variant_setArrayCopy(variant, value.data(), value.size(), &UA_TYPES[UA_TYPES_INT16]);

      else if constexpr (std::is_same<T, std::vector<uint16_t>>::value)
        UA_Variant_setArrayCopy(variant, value.data(), value.size(), &UA_TYPES[UA_TYPES_UINT16]);

      else if constexpr (std::is_same<T, std::vector<int32_t>>::value)
        UA_Variant_setArrayCopy(variant, value.data(), value.size(), &UA_TYPES[UA_TYPES_INT32]);

      else if constexpr (std::is_same<T, std::vector<uint32_t>>::value)
        UA_Variant_setArrayCopy(variant, value.data(), value.size(), &UA_TYPES[UA_TYPES_UINT32]);

      else if constexpr (std::is_same<T, std::vector<int64_t>>::value)
        UA_Variant_setArrayCopy(variant, value.data(), value.size(), &UA_TYPES[UA_TYPES_INT64]);

      else if constexpr (std::is_same<T, std::vector<uint64_t>>::value)
        UA_Variant_setArrayCopy(variant, value.data(), value.size(), &UA_TYPES[UA_TYPES_UINT64]);

      else if constexpr (std::is_same<T, std::vector<float>>::value)
        UA_Variant_setArrayCopy(variant, value.data(), value.size(), &UA_TYPES[UA_TYPES_FLOAT]);

      else if constexpr (std::is_same<T, std::vector<double>>::value)
        UA_Variant_setArrayCopy(variant, value.data(), value.size(), &UA_TYPES[UA_TYPES_DOUBLE]);

      else if constexpr (std::is_same<T, std::vector<std::string>>::value)
      {
        std::vector<UA_String> copy;

        copy.reserve(value.size());

        for (size_t i = 0; i < value.size(); ++i)
        {
          copy.push_back(UA_String_fromChars(value[i].c_str()));
        }

        UA_Variant_setArrayCopy(variant, copy.data(), copy.size(), &UA_TYPES[UA_TYPES_STRING]);

        for (size_t i = 0; i < copy.size(); ++i)
        {
          UA_String_deleteMembers(&copy[i]);
        }
      }

      // ELSE

      else throw std::invalid_argument("Invalid data type!");
    }
  };
}
