#pragma once

#include <reopen62541-cli/Client.h>
#include <reopen62541-cli/Variant.h>

#include <reopen62541/variant.h>

#using <System.dll>

using namespace System;
using namespace System::Runtime::InteropServices;

namespace UA
{
  private ref class ClientMethodCallbackAdapter : public IDisposable
  {
  public:

    property UA::NativeClientMethodRequestCallback NativeRequestCallback
    {
      UA::NativeClientMethodRequestCallback get() { return native_request_callback; }
    }

    property UA::NativeClientMethodResponseCallback NativeResponseCallback
    {
      UA::NativeClientMethodResponseCallback get() { return native_response_callback; }
    }

    ClientMethodCallbackAdapter(Action<UA::Variant^>^ request, Action<UA::Variant^>^ response);
    ~ClientMethodCallbackAdapter();
    !ClientMethodCallbackAdapter();

  private:

    Action<UA::Variant^>^ managed_request;
    Action<UA::Variant^>^ managed_response;

    GCHandle native_request;
    GCHandle native_response;

    UA::ManagedClientMethodRequestCallback^ managed_request_callback;
    UA::ManagedClientMethodResponseCallback^ managed_response_callback;

    UA::NativeClientMethodRequestCallback native_request_callback;
    UA::NativeClientMethodResponseCallback native_response_callback;

    void RequestCallback(ua::variant& input);
    void ResponseCallback(const ua::variant& output);
  };
}
