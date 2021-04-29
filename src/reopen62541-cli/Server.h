#pragma once

#include <reopen62541-cli/Argument.h>
#include <reopen62541-cli/Convert.h>
#include <reopen62541-cli/Log.h>
#include <reopen62541-cli/Variant.h>

#include <reopen62541/server.h>

#using <System.dll>

using namespace System;

namespace UA
{
  typedef void (*NativeServerVariableGetterCallback)(ua::variant& output);
  typedef void (*NativeServerVariableSetterCallback)(const ua::variant& input);
  typedef void (*NativeServerMethodCallback)(const ua::variant& input, ua::variant& output);

  delegate void ManagedServerVariableGetterCallback(ua::variant& output);
  delegate void ManagedServerVariableSetterCallback(const ua::variant& input);
  delegate void ManagedServerMethodCallback(const ua::variant& input, ua::variant& output);

  public ref class Server : public IDisposable
  {
  public:

    event EventHandler<LogEventArgs^>^ LogChanged;

    Server(int portnumber, String^ hostname, String^ name, String^ uri);
    Server();
    ~Server();
    !Server();

    void Run();
    void Shutdown();

    void AddFolder(
      String^ name,
      String^ description,
      ... array<String^>^ path);

    void AddObject(
      String^ name,
      String^ description,
      ... array<String^>^ path);

    generic<class T>
    void AddVariable(
      String^ name,
      String^ description,
      array<String^>^ path,
      Func<T>^ getter,
      Action<T>^ setter);

    generic<class T>
    void AddVariable(
      String^ name,
      String^ description,
      array<String^>^ path,
      Action<UA::Variant^>^ getter,
      Action<UA::Variant^>^ setter);

    void AddMethod(
      String^ name,
      String^ description,
      array<String^>^ path,
      array<UA::Argument^>^ inputs,
      array<UA::Argument^>^ outputs,
      Action<UA::Variant^, UA::Variant^>^ action);

  protected:

    virtual void OnLogChanged(UA::LogLevel level, UA::LogCategory category, String^ message) {}

  private:

    bool disposed;

    ua::server* server;

    void LogCallback(UA::LogLevel level, UA::LogCategory category, String^ message);
  };
}
