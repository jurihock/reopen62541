#pragma once

#include <open62541.h>

#include <string>

#using <System.dll>

using namespace System;

namespace UA
{
  public enum class LogLevel
  {
    Trace = UA_LOGLEVEL_TRACE,
    Debug = UA_LOGLEVEL_DEBUG,
    Info = UA_LOGLEVEL_INFO,
    Warning = UA_LOGLEVEL_WARNING,
    Error = UA_LOGLEVEL_ERROR,
    Fatal = UA_LOGLEVEL_FATAL,
  };

  public enum class LogCategory
  {
    Network = UA_LOGCATEGORY_NETWORK,
    SecureChannel = UA_LOGCATEGORY_SECURECHANNEL,
    Session = UA_LOGCATEGORY_SESSION,
    Server = UA_LOGCATEGORY_SERVER,
    Client = UA_LOGCATEGORY_CLIENT,
    Userland = UA_LOGCATEGORY_USERLAND,
    SecurityPolicy = UA_LOGCATEGORY_SECURITYPOLICY,
  };

  public ref class LogEventArgs : EventArgs
  {
  public:

    property UA::LogLevel Level
    {
      UA::LogLevel get() { return level; }
    }

    property UA::LogCategory Category
    {
      UA::LogCategory get() { return category; }
    }

    property String^ Message
    {
      String^ get() { return message; }
    }

    LogEventArgs(UA::LogLevel level, UA::LogCategory category, String^ message) :
      level(level),
      category(category),
      message(message)
    {
    }

  private:

    UA::LogLevel level;
    UA::LogCategory category;
    String^ message;
  };

  delegate void ManagedLogCallback(UA_LogLevel level, UA_LogCategory category, const std::string& message);
  typedef void (*NativeLogCallback)(UA_LogLevel level, UA_LogCategory category, const std::string& message);
}
