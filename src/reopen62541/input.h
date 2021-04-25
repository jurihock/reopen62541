#pragma once

#include <string>
#include <vector>

#include <open62541.h>

namespace ua
{
  struct input
  {
    const UA_Variant* variant;
    const size_t size;

    input(const UA_Variant* variant, size_t size) : variant(variant), size(size) {}

    input operator[](const size_t index) const { return input(variant + index, size); }

    template<typename T> operator T() const { return get<T>(); }
    template<typename T> void operator>>(T& value) const { value = get<T>(); }

    template<typename T> T get() const
    {
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
        const UA_String* src = static_cast<UA_String*>(variant->data);
        const std::string dst(reinterpret_cast<char*>(src->data), src->length);
        return dst;
      }

      if constexpr (std::is_same<T, std::wstring>::value)
      {
        const UA_String* src = static_cast<UA_String*>(variant->data);
        const std::wstring dst(reinterpret_cast<wchar_t*>(src->data), src->length);
        return dst;
      }

      // VECTORS

      // (TODO)

      // ELSE

      throw std::invalid_argument("Invalid data type!");
    }
  };
}
