#pragma once

#include <reopen62541-cli/Convert.h>
#include <reopen62541-cli/Log.h>
#include <reopen62541-cli/Variant.h>

#include <reopen62541/client.h>

#using <System.dll>

using namespace System;

namespace UA
{
  delegate void ManagedClientVariableGetterCallback(const ua::variant& output);
  delegate void ManagedClientVariableSetterCallback(ua::variant& input);
  delegate void ManagedClientMethodRequestCallback(ua::variant& input);
  delegate void ManagedClientMethodResponseCallback(const ua::variant& output);

  typedef void (*NativeClientVariableGetterCallback)(const ua::variant& output);
  typedef void (*NativeClientVariableSetterCallback)(ua::variant& input);
  typedef void (*NativeClientMethodRequestCallback)(ua::variant& input);
  typedef void (*NativeClientMethodResponseCallback)(const ua::variant& output);

  public ref class Client : public IDisposable
  {
  public:

    event EventHandler<LogEventArgs^>^ LogChanged;

    Client(int portnumber, String^ hostname);
    Client(int portnumber, String^ hostname, int timeout);
    Client();
    ~Client();
    !Client();

    void Connect();
    void Disconnect();

    generic<class T>
    T Get(
      String^ name,
      array<String^>^ path);

    void Get(
      String^ name,
      array<String^>^ path,
      Action<UA::Variant^>^ getter);

    generic<class T>
    void Set(
      String^ name,
      array<String^>^ path,
      T value);

    void Set(
      String^ name,
      array<String^>^ path,
      Action<UA::Variant^>^ setter);

    void Call(
      String^ name,
      array<String^>^ path,
      Action<UA::Variant^>^ request,
      Action<UA::Variant^>^ response);

  private:

    bool disposed;

    ua::client* client;

    void LogCallback(UA::LogLevel level, UA::LogCategory category, String^ message);
  };
}
