#pragma once

#include <reopen62541/variant.h>

#using <System.dll>

using namespace System;
using namespace System::Runtime::InteropServices;

namespace UA
{
  public ref class Variant
  {
  internal:

    Variant(UA_Variant* variant_writable, const UA_Variant* variant_readable, size_t size, size_t index);
    Variant(ua::variant& variant);
    Variant(const ua::variant& variant);

  public:

    property int Size
    {
      int get();
    }

    property int Index
    {
      int get();
    }

    property UA::Variant^ default[int]
    {
      UA::Variant^ get(int index);
    }

    generic<class T>
    T Get();

    generic<class T>
    void Set(T value);

  private:

    UA_Variant* variant_writable;
    const UA_Variant* variant_readable;
    const size_t variant_size;
    const size_t variant_index;
  };
}
