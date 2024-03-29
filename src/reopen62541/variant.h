#pragma once

#include <reopen62541/convert.h>

#include <open62541.h>

#include <string>
#include <vector>

namespace ua
{
  class variant
  {
  private:

    variant(UA_Variant* variant_writable, const UA_Variant* variant_readable, size_t size, size_t index) :
      variant_writable(variant_writable),
      variant_readable(variant_readable),
      variant_size(size),
      variant_index(index)
    {
      if (index >= size)
      {
        throw std::invalid_argument("Specified variant index is out of bounds!");
      }
    }

  public:

    variant(UA_Variant* variant, size_t size) :
      variant_writable(variant),
      variant_readable(variant),
      variant_size(size),
      variant_index(0)
    {
    }

    variant(const UA_Variant* variant, size_t size) :
      variant_writable(nullptr),
      variant_readable(variant),
      variant_size(size),
      variant_index(0)
    {
    }

    variant(const ua::variant& other) :
      variant_writable(other.variant_writable),
      variant_readable(other.variant_readable),
      variant_size(other.variant_size),
      variant_index(other.variant_index)
    {
    }

    UA_Variant* write() const
    {
      return variant_writable;
    }

    const UA_Variant* read() const
    {
      return variant_readable;
    }

    size_t size() const
    {
      return variant_size;
    }

    size_t index() const
    {
      return variant_index;
    }

    variant operator[](const size_t index) const
    {
      return variant(variant_writable, variant_readable, variant_size, index);
    }

    template<typename T> operator T() const { return get<T>(); }
    template<typename T> void operator=(const T& value) { set<T>(value); }

    template<typename T> T get() const
    {
      if (variant_readable == nullptr)
      {
        throw std::runtime_error("Read operation not permitted in current context!");
      }

      const UA_Variant* variant = variant_readable + variant_index;

      // SCALARS

      if constexpr (std::is_same<T, bool>::value)
        return *(static_cast<bool*>(variant->data));

      if constexpr (std::is_same<T, int8_t>::value)
        return *(static_cast<int8_t*>(variant->data));

      if constexpr (std::is_same<T, uint8_t>::value)
        return *(static_cast<uint8_t*>(variant->data));

      if constexpr (std::is_same<T, int16_t>::value)
        return *(static_cast<int16_t*>(variant->data));

      if constexpr (std::is_same<T, uint16_t>::value)
        return *(static_cast<uint16_t*>(variant->data));

      if constexpr (std::is_same<T, int32_t>::value)
        return *(static_cast<int32_t*>(variant->data));

      if constexpr (std::is_same<T, uint32_t>::value)
        return *(static_cast<uint32_t*>(variant->data));

      if constexpr (std::is_same<T, int64_t>::value)
        return *(static_cast<int64_t*>(variant->data));

      if constexpr (std::is_same<T, uint64_t>::value)
        return *(static_cast<uint64_t*>(variant->data));

      if constexpr (std::is_same<T, float>::value)
        return *(static_cast<float*>(variant->data));

      if constexpr (std::is_same<T, double>::value)
        return *(static_cast<double*>(variant->data));

      if constexpr (std::is_same<T, std::string>::value)
      {
        const auto src = static_cast<UA_String*>(variant->data);
        const auto dst = ua::convert::to_string(reinterpret_cast<char*>(src->data), src->length);
        return dst;
      }

      if constexpr (std::is_same<T, std::wstring>::value)
      {
        const auto src = static_cast<UA_String*>(variant->data);
        const auto dst = ua::convert::to_wstring(reinterpret_cast<char*>(src->data), src->length);
        return dst;
      }

      // VECTORS

      if constexpr (std::is_same<T, std::vector<bool>>::value)
      {
        const auto size = variant->arrayLength;
        const auto src = static_cast<uint8_t*>(variant->data);
        std::vector<bool> dst(size);
        for (size_t i = 0; i < size; ++i)
        {
          dst[i] = src[i];
        }
        return dst;
      }

      if constexpr (std::is_same<T, std::vector<int8_t>>::value)
      {
        const auto size = variant->arrayLength;
        const auto src = static_cast<int8_t*>(variant->data);
        std::vector<int8_t> dst(src, src + size);
        return dst;
      }

      if constexpr (std::is_same<T, std::vector<uint8_t>>::value)
      {
        const auto size = variant->arrayLength;
        const auto src = static_cast<uint8_t*>(variant->data);
        std::vector<uint8_t> dst(src, src + size);
        return dst;
      }

      if constexpr (std::is_same<T, std::vector<int16_t>>::value)
      {
        const auto size = variant->arrayLength;
        const auto src = static_cast<int16_t*>(variant->data);
        std::vector<int16_t> dst(src, src + size);
        return dst;
      }

      if constexpr (std::is_same<T, std::vector<uint16_t>>::value)
      {
        const auto size = variant->arrayLength;
        const auto src = static_cast<uint16_t*>(variant->data);
        std::vector<uint16_t> dst(src, src + size);
        return dst;
      }

      if constexpr (std::is_same<T, std::vector<int32_t>>::value)
      {
        const auto size = variant->arrayLength;
        const auto src = static_cast<int32_t*>(variant->data);
        std::vector<int32_t> dst(src, src + size);
        return dst;
      }

      if constexpr (std::is_same<T, std::vector<uint32_t>>::value)
      {
        const auto size = variant->arrayLength;
        const auto src = static_cast<uint32_t*>(variant->data);
        std::vector<uint32_t> dst(src, src + size);
        return dst;
      }

      if constexpr (std::is_same<T, std::vector<int64_t>>::value)
      {
        const auto size = variant->arrayLength;
        const auto src = static_cast<int64_t*>(variant->data);
        std::vector<int64_t> dst(src, src + size);
        return dst;
      }

      if constexpr (std::is_same<T, std::vector<uint64_t>>::value)
      {
        const auto size = variant->arrayLength;
        const auto src = static_cast<uint64_t*>(variant->data);
        std::vector<uint64_t> dst(src, src + size);
        return dst;
      }

      if constexpr (std::is_same<T, std::vector<float>>::value)
      {
        const auto size = variant->arrayLength;
        const auto src = static_cast<float*>(variant->data);
        std::vector<float> dst(src, src + size);
        return dst;
      }

      if constexpr (std::is_same<T, std::vector<double>>::value)
      {
        const auto size = variant->arrayLength;
        const auto src = static_cast<double*>(variant->data);
        std::vector<double> dst(src, src + size);
        return dst;
      }

      if constexpr (std::is_same<T, std::vector<std::string>>::value)
      {
        const auto size = variant->arrayLength;
        const auto input = static_cast<UA_String*>(variant->data);
        std::vector<std::string> output;
        output.reserve(size);

        for (size_t i = 0; i < size; ++i)
        {
          const auto& src = input[i];
          const auto dst = ua::convert::to_string(reinterpret_cast<char*>(src.data), src.length);
          output.push_back(dst);
        }

        return output;
      }

      if constexpr (std::is_same<T, std::vector<std::wstring>>::value)
      {
        const auto size = variant->arrayLength;
        const auto input = static_cast<UA_String*>(variant->data);
        std::vector<std::wstring> output;
        output.reserve(size);

        for (size_t i = 0; i < size; ++i)
        {
          const auto& src = input[i];
          const auto dst = ua::convert::to_wstring(reinterpret_cast<char*>(src.data), src.length);
          output.push_back(dst);
        }

        return output;
      }

      // ELSE

      throw std::invalid_argument("Invalid data type!");
    }

    template<typename T> void set(const T& value)
    {
      if (variant_writable == nullptr)
      {
        throw std::runtime_error("Write operation not permitted in current context!");
      }

      UA_Variant* variant = variant_writable + variant_index;

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
        const auto src = value + '\0';
        auto dst = UA_String_fromChars(src.c_str());
        UA_Variant_setScalarCopy(variant, &dst, &UA_TYPES[UA_TYPES_STRING]);
        UA_String_deleteMembers(&dst);
      }

      else if constexpr (std::is_same<T, std::wstring>::value)
      {
        const auto src = ua::convert::to_string(value) + '\0';
        auto dst = UA_String_fromChars(src.c_str());
        UA_Variant_setScalarCopy(variant, &dst, &UA_TYPES[UA_TYPES_STRING]);
        UA_String_deleteMembers(&dst);
      }

      // VECTORS

      else if constexpr (std::is_same<T, std::vector<bool>>::value)
      {
        std::vector<uint8_t> copy(value.size());
        for (size_t i = 0; i < value.size(); ++i)
        {
          copy[i] = value[i];
        }
        UA_Variant_setArrayCopy(variant, copy.data(), copy.size(), &UA_TYPES[UA_TYPES_BOOLEAN]);
      }

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
          const auto src = value[i] + '\0';
          const auto dst = UA_String_fromChars(src.c_str());
          copy.push_back(dst);
        }

        UA_Variant_setArrayCopy(variant, copy.data(), copy.size(), &UA_TYPES[UA_TYPES_STRING]);

        for (size_t i = 0; i < copy.size(); ++i)
        {
          UA_String_deleteMembers(&copy[i]);
        }
      }

      else if constexpr (std::is_same<T, std::vector<std::wstring>>::value)
      {
        std::vector<UA_String> copy;
        copy.reserve(value.size());

        for (size_t i = 0; i < value.size(); ++i)
        {
          const auto src = ua::convert::to_string(value[i]) + '\0';
          const auto dst = UA_String_fromChars(src.c_str());
          copy.push_back(dst);
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

    private:

      UA_Variant* variant_writable;
      const UA_Variant* variant_readable;
      const size_t variant_size;
      const size_t variant_index;
  };
}
