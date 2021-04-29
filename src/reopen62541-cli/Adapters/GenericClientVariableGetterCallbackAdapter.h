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
  private ref class GenericClientVariableGetterCallbackAdapter : public IDisposable
  {
  public:

    property T Value
    {
      T get() { return this->value; }
      void set(T value) { this->value = value; }
    }

    GenericClientVariableGetterCallbackAdapter();
    ~GenericClientVariableGetterCallbackAdapter();
    !GenericClientVariableGetterCallbackAdapter();

    UA::NativeClientVariableGetterCallback GetNativeGetterCallback();

  private:

    T value;

    GCHandle native_getter;

    UA::ManagedClientVariableGetterCallback^ managed_getter_callback;

    UA::NativeClientVariableGetterCallback native_getter_callback;

    void GetterCallback(const ua::variant& output);
  };
}
