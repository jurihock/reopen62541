#pragma once

#include <reopen62541-cli/Convert.h>
#include <reopen62541-cli/Log.h>

#using <System.dll>

using namespace System;
using namespace System::Runtime::InteropServices;

namespace UA
{
  private ref class LogCallbackAdapter : public IDisposable
  {
  public:

    property UA::NativeLogCallback NativeLogCallback
    {
      UA::NativeLogCallback get() { return native_log_callback; }
    }

    LogCallbackAdapter(Action<UA::LogLevel, UA::LogCategory, String^>^ log);
    ~LogCallbackAdapter();
    !LogCallbackAdapter();

  private:

    Action<UA::LogLevel, UA::LogCategory, String^>^ managed_log;

    GCHandle native_log;

    UA::ManagedLogCallback^ managed_log_callback;
    UA::NativeLogCallback native_log_callback;

    void ActionCallback(UA_LogLevel level, UA_LogCategory category, const std::string& message);
  };
}
