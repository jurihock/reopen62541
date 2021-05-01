#pragma once

#define nameof(T) #T
#define typeof(T) T::typeid

#include <open62541.h>

#include <msclr\marshal.h>
#include <msclr\marshal_cppstd.h>

#using <System.dll>

using namespace System;
using namespace System::Collections::Generic;

namespace UA
{
  private ref class Convert abstract sealed
  {
  public:

    static String^ ToString(const std::string& value)
    {
      return msclr::interop::marshal_as<String^>(value);
    }

    static String^ ToString(const std::wstring& value)
    {
      return msclr::interop::marshal_as<String^>(value);
    }

    static std::string ToStdString(String^ value)
    {
      return msclr::interop::marshal_as<std::string>(value);
    }

    static std::wstring ToStdWideString(String^ value)
    {
      return msclr::interop::marshal_as<std::wstring>(value);
    }

    static std::vector<std::string> ToStdStringVector(... array<String^>^ values)
    {
      if (values == nullptr)
      {
        throw gcnew ArgumentNullException(nameof(values));
      }

      std::vector<std::string> values_std(values->LongLength);

      for (long i = 0; i < values->LongLength; ++i)
      {
        values_std[i] = UA::Convert::ToStdString(values[i]);
      }

      return values_std;
    }

    static std::vector<std::wstring> ToStdWideStringVector(... array<String^>^ values)
    {
      if (values == nullptr)
      {
        throw gcnew ArgumentNullException(nameof(values));
      }

      std::vector<std::wstring> values_std(values->LongLength);

      for (long i = 0; i < values->LongLength; ++i)
      {
        values_std[i] = UA::Convert::ToStdWideString(values[i]);
      }

      return values_std;
    }

    generic<class Tout, class Tin>
    where Tout : IConvertible
    where Tin : IConvertible
    static array<Tout>^ ToArray(... array<Tin>^ values)
    {
      if (values == nullptr)
      {
        throw gcnew ArgumentNullException(nameof(values));
      }

      auto result = gcnew array<Tout>(values->LongLength);

      for (long i = 0; i < values->LongLength; i++)
      {
        result[i] = (Tout)values[i];
      }

      return result;
    }

    generic<class T>
    static UInt32 ToUaDataType()
    {
      auto types = gcnew Dictionary<Type^, UInt32>();

      types->Add(Boolean::typeid, UA_TYPES_BOOLEAN);
      types->Add(SByte::typeid, UA_TYPES_SBYTE);
      types->Add(Byte::typeid, UA_TYPES_BYTE);
      types->Add(Int16::typeid, UA_TYPES_INT16);
      types->Add(UInt16::typeid, UA_TYPES_UINT16);
      types->Add(Int32::typeid, UA_TYPES_INT32);
      types->Add(UInt32::typeid, UA_TYPES_UINT32);
      types->Add(Int64::typeid, UA_TYPES_INT64);
      types->Add(UInt64::typeid, UA_TYPES_UINT64);
      types->Add(Single::typeid, UA_TYPES_FLOAT);
      types->Add(Double::typeid, UA_TYPES_DOUBLE);
      types->Add(String::typeid, UA_TYPES_STRING);

      auto type = T::typeid->IsArray
        ? T::typeid->GetElementType()
        : T::typeid;

      if (!types->ContainsKey(type))
      {
        throw gcnew Exception();
      }

      return types[type];
    }

    generic<class T>
    static Int32 ToUaValueRank()
    {
      return T::typeid->IsArray
        ? UA_VALUERANK_SCALAR_OR_ONE_DIMENSION
        : UA_VALUERANK_SCALAR;
    }
  };
}
