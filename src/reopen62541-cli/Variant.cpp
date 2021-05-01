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

  // VECTORS

  else if (T::typeid->IsArray && T::typeid->GetElementType() == Boolean::typeid)
  {
    auto native_values = variant.get<std::vector<bool>>();
    auto managed_values = gcnew array<Boolean>(native_values.size());
    for (int i = 0; i < managed_values->Length; ++i)
    {
      managed_values[i] = native_values[i];
    }
    return (T)managed_values;
  }

  else if (T::typeid->IsArray && T::typeid->GetElementType() == SByte::typeid)
  {
    auto native_values = variant.get<std::vector<int8_t>>();
    auto managed_values = gcnew array<Byte>(native_values.size()); // not SByte
    Marshal::Copy(IntPtr(native_values.data()), managed_values, 0, managed_values->Length);
    return (T)managed_values;
  }

  else if (T::typeid->IsArray && T::typeid->GetElementType() == Byte::typeid)
  {
    auto native_values = variant.get<std::vector<uint8_t>>();
    auto managed_values = gcnew array<Byte>(native_values.size());
    Marshal::Copy(IntPtr(native_values.data()), managed_values, 0, managed_values->Length);
    return (T)managed_values;
  }

  else if (T::typeid->IsArray && T::typeid->GetElementType() == Int16::typeid)
  {
    auto native_values = variant.get<std::vector<int16_t>>();
    auto managed_values = gcnew array<Int16>(native_values.size());
    Marshal::Copy(IntPtr(native_values.data()), managed_values, 0, managed_values->Length);
    return (T)managed_values;
  }

  else if (T::typeid->IsArray && T::typeid->GetElementType() == UInt16::typeid)
  {
    auto native_values = variant.get<std::vector<uint16_t>>();
    auto managed_values = gcnew array<Int16>(native_values.size()); // not UInt16
    Marshal::Copy(IntPtr(native_values.data()), managed_values, 0, managed_values->Length);
    return (T)managed_values;
  }

  else if (T::typeid->IsArray && T::typeid->GetElementType() == Int32::typeid)
  {
    auto native_values = variant.get<std::vector<int32_t>>();
    auto managed_values = gcnew array<Int32>(native_values.size());
    Marshal::Copy(IntPtr(native_values.data()), managed_values, 0, managed_values->Length);
    return (T)managed_values;
  }

  else if (T::typeid->IsArray && T::typeid->GetElementType() == UInt32::typeid)
  {
    auto native_values = variant.get<std::vector<uint32_t>>();
    auto managed_values = gcnew array<Int32>(native_values.size()); // not UInt32
    Marshal::Copy(IntPtr(native_values.data()), managed_values, 0, managed_values->Length);
    return (T)managed_values;
  }

  else if (T::typeid->IsArray && T::typeid->GetElementType() == Int64::typeid)
  {
    auto native_values = variant.get<std::vector<int64_t>>();
    auto managed_values = gcnew array<Int64>(native_values.size());
    Marshal::Copy(IntPtr(native_values.data()), managed_values, 0, managed_values->Length);
    return (T)managed_values;
  }

  else if (T::typeid->IsArray && T::typeid->GetElementType() == UInt64::typeid)
  {
    auto native_values = variant.get<std::vector<uint64_t>>();
    auto managed_values = gcnew array<Int64>(native_values.size()); // not UInt64
    Marshal::Copy(IntPtr(native_values.data()), managed_values, 0, managed_values->Length);
    return (T)managed_values;
  }

  else if (T::typeid->IsArray && T::typeid->GetElementType() == Single::typeid)
  {
    auto native_values = variant.get<std::vector<float>>();
    auto managed_values = gcnew array<Single>(native_values.size());
    Marshal::Copy(IntPtr(native_values.data()), managed_values, 0, managed_values->Length);
    return (T)managed_values;
  }

  else if (T::typeid->IsArray && T::typeid->GetElementType() == Double::typeid)
  {
    auto native_values = variant.get<std::vector<double>>();
    auto managed_values = gcnew array<Double>(native_values.size());
    Marshal::Copy(IntPtr(native_values.data()), managed_values, 0, managed_values->Length);
    return (T)managed_values;
  }

  else if (T::typeid->IsArray && T::typeid->GetElementType() == String::typeid)
  {
    auto native_values = variant.get<std::vector<std::wstring>>();
    auto managed_values = gcnew array<String^>(native_values.size());
    for (int i = 0; i < managed_values->Length; ++i)
    {
      managed_values[i] = UA::Convert::ToString(native_values[i]);
    }
    return (T)managed_values;
  }

  // ELSE

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
  }

  else if (T::typeid == SByte::typeid)
  {
    const auto native_value = (int8_t)managed_value;
    variant.set<int8_t>(native_value);
  }

  else if (T::typeid == Byte::typeid)
  {
    const auto native_value = (uint8_t)managed_value;
    variant.set<uint8_t>(native_value);
  }

  else if (T::typeid == Int16::typeid)
  {
    const auto native_value = (int16_t)managed_value;
    variant.set<int16_t>(native_value);
  }

  else if (T::typeid == UInt16::typeid)
  {
    const auto native_value = (uint16_t)managed_value;
    variant.set<uint16_t>(native_value);
  }

  else if (T::typeid == Int32::typeid)
  {
    const auto native_value = (int32_t)managed_value;
    variant.set<int32_t>(native_value);
  }

  else if (T::typeid == UInt32::typeid)
  {
    const auto native_value = (uint32_t)managed_value;
    variant.set<uint32_t>(native_value);
  }

  else if (T::typeid == Int64::typeid)
  {
    const auto native_value = (int64_t)managed_value;
    variant.set<int64_t>(native_value);
  }

  else if (T::typeid == UInt64::typeid)
  {
    const auto native_value = (uint64_t)managed_value;
    variant.set<uint64_t>(native_value);
  }

  else if (T::typeid == Single::typeid)
  {
    const auto native_value = (float)managed_value;
    variant.set<float>(native_value);
  }

  else if (T::typeid == Double::typeid)
  {
    const auto native_value = (double)managed_value;
    variant.set<double>(native_value);
  }

  else if (T::typeid == String::typeid)
  {
    const auto native_value = UA::Convert::ToStdWideString((String^)managed_value);
    variant.set<std::wstring>(native_value);
  }

  // VECTORS

  else if (T::typeid->IsArray && T::typeid->GetElementType() == Boolean::typeid)
  {
    auto managed_values = (array<Boolean>^)managed_value;
    auto native_values = std::vector<bool>(managed_values->Length);
    for (int i = 0; i < managed_values->Length; ++i)
    {
      native_values[i] = managed_values[i];
    }
    variant.set<std::vector<bool>>(native_values);
  }

  else if (T::typeid->IsArray && T::typeid->GetElementType() == SByte::typeid)
  {
    auto managed_values = (array<Byte>^)managed_value; // not SByte
    auto native_values = std::vector<int8_t>(managed_values->Length);
    Marshal::Copy(managed_values, 0, IntPtr(native_values.data()), managed_values->Length);
    variant.set<std::vector<int8_t>>(native_values);
  }

  else if (T::typeid->IsArray && T::typeid->GetElementType() == Byte::typeid)
  {
    auto managed_values = (array<Byte>^)managed_value;
    auto native_values = std::vector<uint8_t>(managed_values->Length);
    Marshal::Copy(managed_values, 0, IntPtr(native_values.data()), managed_values->Length);
    variant.set<std::vector<uint8_t>>(native_values);
  }

  else if (T::typeid->IsArray && T::typeid->GetElementType() == Int16::typeid)
  {
    auto managed_values = (array<Int16>^)managed_value;
    auto native_values = std::vector<int16_t>(managed_values->Length);
    Marshal::Copy(managed_values, 0, IntPtr(native_values.data()), managed_values->Length);
    variant.set<std::vector<int16_t>>(native_values);
  }

  else if (T::typeid->IsArray && T::typeid->GetElementType() == UInt16::typeid)
  {
    auto managed_values = (array<Int16>^)managed_value; // not UInt16
    auto native_values = std::vector<uint16_t>(managed_values->Length);
    Marshal::Copy(managed_values, 0, IntPtr(native_values.data()), managed_values->Length);
    variant.set<std::vector<uint16_t>>(native_values);
  }

  else if (T::typeid->IsArray && T::typeid->GetElementType() == Int32::typeid)
  {
    auto managed_values = (array<Int32>^)managed_value;
    auto native_values = std::vector<int32_t>(managed_values->Length);
    Marshal::Copy(managed_values, 0, IntPtr(native_values.data()), managed_values->Length);
    variant.set<std::vector<int32_t>>(native_values);
  }

  else if (T::typeid->IsArray && T::typeid->GetElementType() == UInt32::typeid)
  {
    auto managed_values = (array<Int32>^)managed_value; // not UInt32
    auto native_values = std::vector<uint32_t>(managed_values->Length);
    Marshal::Copy(managed_values, 0, IntPtr(native_values.data()), managed_values->Length);
    variant.set<std::vector<uint32_t>>(native_values);
  }

  else if (T::typeid->IsArray && T::typeid->GetElementType() == Int64::typeid)
  {
    auto managed_values = (array<Int64>^)managed_value;
    auto native_values = std::vector<int64_t>(managed_values->Length);
    Marshal::Copy(managed_values, 0, IntPtr(native_values.data()), managed_values->Length);
    variant.set<std::vector<int64_t>>(native_values);
  }

  else if (T::typeid->IsArray && T::typeid->GetElementType() == UInt64::typeid)
  {
    auto managed_values = (array<Int64>^)managed_value; // not UInt64
    auto native_values = std::vector<uint64_t>(managed_values->Length);
    Marshal::Copy(managed_values, 0, IntPtr(native_values.data()), managed_values->Length);
    variant.set<std::vector<uint64_t>>(native_values);
  }

  else if (T::typeid->IsArray && T::typeid->GetElementType() == Single::typeid)
  {
    auto managed_values = (array<Single>^)managed_value;
    auto native_values = std::vector<float>(managed_values->Length);
    Marshal::Copy(managed_values, 0, IntPtr(native_values.data()), managed_values->Length);
    variant.set<std::vector<float>>(native_values);
  }

  else if (T::typeid->IsArray && T::typeid->GetElementType() == Double::typeid)
  {
    auto managed_values = (array<Double>^)managed_value;
    auto native_values = std::vector<double>(managed_values->Length);
    Marshal::Copy(managed_values, 0, IntPtr(native_values.data()), managed_values->Length);
    variant.set<std::vector<double>>(native_values);
  }

  else if (T::typeid->IsArray && T::typeid->GetElementType() == String::typeid)
  {
    auto managed_values = (array<String^>^)managed_value;
    auto native_values = std::vector<std::wstring>(managed_values->Length);
    for (int i = 0; i < managed_values->Length; ++i)
    {
      native_values[i] = UA::Convert::ToStdWideString(managed_values[i]);
    }
    variant.set<std::vector<std::wstring>>(native_values);
  }

  // ELSE

  else throw gcnew NotSupportedException();
}
