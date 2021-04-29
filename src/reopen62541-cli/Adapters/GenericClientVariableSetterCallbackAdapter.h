#pragma once

#include <reopen62541-cli/Client.h>
#include <reopen62541-cli/Variant.h>

#include <reopen62541/variant.h>

#using <System.dll>

using namespace System;
using namespace System::Runtime::InteropServices;

namespace UA
{
  generic<class T>
  private ref class GenericClientVariableSetterCallbackAdapter : public IDisposable
  {
  public:

    property T Value
    {
      T get() { return this->value; }
      void set(T value) { this->value = value; }
    }

    GenericClientVariableSetterCallbackAdapter(T value);
    ~GenericClientVariableSetterCallbackAdapter();
    !GenericClientVariableSetterCallbackAdapter();

    UA::NativeClientVariableSetterCallback GetNativeSetterCallback();

  private:

    T value;

    GCHandle native_setter;

    UA::ManagedClientVariableSetterCallback^ managed_setter_callback;

    UA::NativeClientVariableSetterCallback native_setter_callback;

    void SetterCallback(ua::variant& input);
  };
}
