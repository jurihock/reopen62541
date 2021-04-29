#pragma once

#include <reopen62541-cli/Client.h>
#include <reopen62541-cli/Variant.h>

#include <reopen62541/variant.h>

#using <System.dll>

using namespace System;
using namespace System::Runtime::InteropServices;

namespace UA
{
  private ref class ClientVariableSetterCallbackAdapter : public IDisposable
  {
  public:

    property UA::NativeClientVariableSetterCallback NativeSetterCallback
    {
      UA::NativeClientVariableSetterCallback get() { return native_setter_callback; }
    }

    ClientVariableSetterCallbackAdapter(Action<UA::Variant^>^ setter);
    ~ClientVariableSetterCallbackAdapter();
    !ClientVariableSetterCallbackAdapter();

  private:

    Action<UA::Variant^>^ managed_setter;

    GCHandle native_setter;

    UA::ManagedClientVariableSetterCallback^ managed_setter_callback;
    UA::NativeClientVariableSetterCallback native_setter_callback;

    void SetterCallback(ua::variant& input);
  };
}
