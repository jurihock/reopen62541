#pragma once

#include <reopen62541-cli/Convert.h>
#include <reopen62541-cli/Exception.h>
#include <reopen62541-cli/Log.h>
#include <reopen62541-cli/Variant.h>

#include <reopen62541/client.h>

#using <System.dll>

using namespace System;

namespace UA
{
  typedef void (*NativeClientVariableGetterCallback)(const ua::variant& output);
  typedef void (*NativeClientVariableSetterCallback)(ua::variant& input);
  typedef void (*NativeClientMethodRequestCallback)(ua::variant& input);
  typedef void (*NativeClientMethodResponseCallback)(const ua::variant& output);

  delegate void ManagedClientVariableGetterCallback(const ua::variant& output);
  delegate void ManagedClientVariableSetterCallback(ua::variant& input);
  delegate void ManagedClientMethodRequestCallback(ua::variant& input);
  delegate void ManagedClientMethodResponseCallback(const ua::variant& output);

  public ref class Client : public IDisposable
  {
  public:

    property bool IsConnected
    {
      bool get() { return client != nullptr && client->connected(); }
    }

    event EventHandler<LogEventArgs^>^ LogChanged;

    Client(int portnumber, String^ hostname, int timeout);
    Client(int portnumber, String^ hostname);
    Client(int portnumber);   
    Client();
    ~Client();
    !Client();

    void Connect();
    void Disconnect();

    void Sync(int timeout);

    generic<class T>
    T Get(
      String^ name,
      array<String^>^ path);

    generic<class T>
    void Set(
      String^ name,
      array<String^>^ path,
      T value);

    void Read(
      String^ name,
      array<String^>^ path,
      Action<UA::Variant^>^ getter);

    void Write(
      String^ name,
      array<String^>^ path,
      Action<UA::Variant^>^ setter);

    void Call(
      String^ name,
      array<String^>^ path,
      Action<UA::Variant^>^ request,
      Action<UA::Variant^>^ response);

  protected:

    virtual void OnLogChanged(UA::LogLevel level, UA::LogCategory category, String^ message) {}

  private:

    bool disposed;

    ua::client* client;

    Client(ua::client* client);

    void LogCallback(UA::LogLevel level, UA::LogCategory category, String^ message);
  };
}
