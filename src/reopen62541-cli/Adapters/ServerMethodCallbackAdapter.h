#pragma once

#include <reopen62541-cli/Server.h>
#include <reopen62541-cli/Variant.h>

#include <reopen62541/variant.h>

#using <System.dll>

using namespace System;
using namespace System::Runtime::InteropServices;

namespace UA
{
  private ref class ServerMethodCallbackAdapter : public IDisposable
  {
  public:

    property UA::NativeServerMethodCallback NativeActionCallback
    {
      UA::NativeServerMethodCallback get() { return native_action_callback; }
    }

    ServerMethodCallbackAdapter(Action<UA::Variant^, UA::Variant^>^ action);
    ~ServerMethodCallbackAdapter();
    !ServerMethodCallbackAdapter();

  private:

    Action<UA::Variant^, UA::Variant^>^ managed_action;

    GCHandle native_action;

    UA::ManagedServerMethodCallback^ managed_action_callback;
    UA::NativeServerMethodCallback native_action_callback;

    void ActionCallback(const ua::variant& input, ua::variant& output);
  };
}
