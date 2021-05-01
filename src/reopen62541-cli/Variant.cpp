#include <reopen62541-cli/Variant.h>

#include <reopen62541-cli/Convert.h>

UA::Variant::Variant(UA_Variant* variant_writable, const UA_Variant* variant_readable, size_t size, size_t index) :
  variant_writable(variant_writable != nullptr ? variant_writable : nullptr),
  variant_readable(variant_readable != nullptr ? variant_readable : nullptr),
  variant_size(size),
  variant_index(index)
{
  if (index >= variant_size)
  {
    throw gcnew IndexOutOfRangeException("Specified variant index is out of bounds!");
  }
}

UA::Variant::Variant(ua::variant& variant) :
  variant_writable(variant.write()),
  variant_readable(variant.read()),
  variant_size(variant.size()),
  variant_index(variant.index())
{
}

UA::Variant::Variant(const ua::variant& variant) :
  variant_writable(variant.write()),
  variant_readable(variant.read()),
  variant_size(variant.size()),
  variant_index(variant.index())
{
}

UA::Variant^ UA::Variant::default::get(int index)
{
  return gcnew UA::Variant(variant_writable, variant_readable, variant_size, index);
}

int UA::Variant::Size::get()
{
  return static_cast<int>(variant_size);
}

int UA::Variant::Index::get()
{
  return static_cast<int>(variant_index);
}

generic<class T>
T UA::Variant::Get()
{
  if (variant_readable == nullptr)
  {
    throw gcnew NullReferenceException();
  }

  const ua::variant variant(variant_readable, variant_size);

  // SCALARS

  if (T::typeid == Boolean::typeid)
  {
    return (T)variant.get<bool>();
  }

  if (T::typeid == SByte::typeid)
  {
    return (T)variant.get<int8_t>();
  }

  if (T::typeid == Byte::typeid)
  {
    return (T)variant.get<uint8_t>();
  }

  if (T::typeid == Int16::typeid)
  {
    return (T)variant.get<int16_t>();
  }

  if (T::typeid == UInt16::typeid)
  {
    return (T)variant.get<uint16_t>();
  }

  if (T::typeid == Int32::typeid)
  {
    return (T)variant.get<int32_t>();
  }

  if (T::typeid == UInt32::typeid)
  {
    return (T)variant.get<uint32_t>();
  }

  if (T::typeid == Int64::typeid)
  {
    return (T)variant.get<int64_t>();
  }

  if (T::typeid == UInt64::typeid)
  {
    return (T)variant.get<uint64_t>();
  }

  if (T::typeid == Single::typeid)
  {
    return (T)variant.get<float>();
  }

  if (T::typeid == Double::typeid)
  {
    return (T)variant.get<double>();
  }

  if (T::typeid == String::typeid)
  {
    const auto native_value = variant.get<std::wstring>();
    const auto managed_value = UA::Convert::ToString(native_value);
    return (T)managed_value;
  }

  throw gcnew NotSupportedException();
}

generic<class T>
void UA::Variant::Set(T managed_value)
{
  if (variant_writable == nullptr)
  {
    throw gcnew NullReferenceException();
  }

  ua::variant variant(variant_writable, variant_size);

  // SCALARS

  if (T::typeid == Boolean::typeid)
  {
    const auto native_value = (bool)managed_value;
    variant.set<bool>(native_value);
    return;
  }

  if (T::typeid == SByte::typeid)
  {
    const auto native_value = (int8_t)managed_value;
    variant.set<int8_t>(native_value);
    return;
  }

  if (T::typeid == Byte::typeid)
  {
    const auto native_value = (uint8_t)managed_value;
    variant.set<uint8_t>(native_value);
    return;
  }

  if (T::typeid == Int16::typeid)
  {
    const auto native_value = (int16_t)managed_value;
    variant.set<int16_t>(native_value);
    return;
  }

  if (T::typeid == UInt16::typeid)
  {
    const auto native_value = (uint16_t)managed_value;
    variant.set<uint16_t>(native_value);
    return;
  }

  if (T::typeid == Int32::typeid)
  {
    const auto native_value = (int32_t)managed_value;
    variant.set<int32_t>(native_value);
    return;
  }

  if (T::typeid == UInt32::typeid)
  {
    const auto native_value = (uint32_t)managed_value;
    variant.set<uint32_t>(native_value);
    return;
  }

  if (T::typeid == Int64::typeid)
  {
    const auto native_value = (int64_t)managed_value;
    variant.set<int64_t>(native_value);
    return;
  }

  if (T::typeid == UInt64::typeid)
  {
    const auto native_value = (uint64_t)managed_value;
    variant.set<uint64_t>(native_value);
    return;
  }

  if (T::typeid == Single::typeid)
  {
    const auto native_value = (float)managed_value;
    variant.set<float>(native_value);
    return;
  }

  if (T::typeid == Double::typeid)
  {
    const auto native_value = (double)managed_value;
    variant.set<double>(native_value);
    return;
  }

  if (T::typeid == String::typeid)
  {
    const auto native_value = UA::Convert::ToStdWideString((String^)managed_value);
    variant.set<std::wstring>(native_value);
    return;
  }

  throw gcnew NotSupportedException();
}
