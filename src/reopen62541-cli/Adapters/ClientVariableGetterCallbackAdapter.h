#pragma once

#include <reopen62541-cli/Client.h>
#include <reopen62541-cli/Variant.h>

#include <reopen62541/variant.h>

#using <System.dll>

using namespace System;
using namespace System::Runtime::InteropServices;

namespace UA
{
  private ref class ClientVariableGetterCallbackAdapter : public IDisposable
  {
  public:

    property UA::NativeClientVariableGetterCallback NativeGetterCallback
    {
      UA::NativeClientVariableGetterCallback get() { return native_getter_callback; }
    }

    ClientVariableGetterCallbackAdapter(Action<UA::Variant^>^ getter);
    ~ClientVariableGetterCallbackAdapter();
    !ClientVariableGetterCallbackAdapter();

  private:

    Action<UA::Variant^>^ managed_getter;

    GCHandle native_getter;

    UA::ManagedClientVariableGetterCallback^ managed_getter_callback;
    UA::NativeClientVariableGetterCallback native_getter_callback;

    void GetterCallback(const ua::variant& output);
  };
}
