#pragma once

#include <reopen62541-cli/Server.h>
#include <reopen62541-cli/Variant.h>

#include <reopen62541/variant.h>

#using <System.dll>

using namespace System;
using namespace System::Runtime::InteropServices;

namespace UA
{
  private ref class ServerVariableCallbackAdapter : public IDisposable
  {
  public:

    ServerVariableCallbackAdapter(Action<UA::Variant^>^ getter, Action<UA::Variant^>^ setter);
    ~ServerVariableCallbackAdapter();
    !ServerVariableCallbackAdapter();

    UA::NativeServerVariableGetterCallback GetNativeGetterCallback();
    UA::NativeServerVariableSetterCallback GetNativeSetterCallback();

  private:

    Action<UA::Variant^>^ managed_getter;
    Action<UA::Variant^>^ managed_setter;

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
