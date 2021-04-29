#pragma once

#include <reopen62541-cli/Server.h>
#include <reopen62541-cli/Variant.h>

#include <reopen62541/variant.h>

#using <System.dll>

using namespace System;
using namespace System::Runtime::InteropServices;

namespace UA
{
  generic<class T>
  private ref class GenericServerVariableCallbackAdapter : public IDisposable
  {
  public:

    property UA::NativeServerVariableGetterCallback NativeGetterCallback
    {
      UA::NativeServerVariableGetterCallback get() { return native_getter_callback; }
    }

    property UA::NativeServerVariableSetterCallback NativeSetterCallback
    {
      UA::NativeServerVariableSetterCallback get() { return native_setter_callback; }
    }

    GenericServerVariableCallbackAdapter(Func<T>^ getter, Action<T>^ setter);
    ~GenericServerVariableCallbackAdapter();
    !GenericServerVariableCallbackAdapter();

  private:

    Func<T>^ managed_getter;
    Action<T>^ managed_setter;

    GCHandle native_getter;
    GCHandle native_setter;

    UA::ManagedServerVariableGetterCallback^ managed_getter_callback;
    UA::ManagedServerVariableSetterCallback^ managed_setter_callback;

    UA::NativeServerVariableGetterCallback native_getter_callback;
    UA::NativeServerVariableSetterCallback native_setter_callback;

    void GetterCallback(ua::variant& output);
    void SetterCallback(const ua::variant& input);
  };
}
