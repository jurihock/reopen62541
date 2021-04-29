#include <reopen62541-cli/Client.h>

#include <reopen62541-cli/Adapters/LogCallbackAdapter.h>
#include <reopen62541-cli/Adapters/GenericClientVariableGetterCallbackAdapter.h>
#include <reopen62541-cli/Adapters/GenericClientVariableSetterCallbackAdapter.h>
#include <reopen62541-cli/Adapters/ClientVariableGetterCallbackAdapter.h>
#include <reopen62541-cli/Adapters/ClientVariableSetterCallbackAdapter.h>
#include <reopen62541-cli/Adapters/ClientMethodCallbackAdapter.h>

UA::Client::Client(int portnumber, String^ hostname) :
  disposed(false),
  client(nullptr)
{
  const auto hostname_std = UA::Convert::ToStdString(hostname);

  client = new ua::client(
    portnumber,
    hostname_std);

  auto client_log_callback = gcnew Action<UA::LogLevel, UA::LogCategory, String^>(this, &UA::Client::LogCallback);
  auto client_log_adapter = gcnew UA::LogCallbackAdapter(client_log_callback);
  client->add_log_callback(client_log_adapter->NativeLogCallback);
}

UA::Client::Client(int portnumber, String^ hostname, int timeout) :
  disposed(false),
  client(nullptr)
{
  const auto hostname_std = UA::Convert::ToStdString(hostname);

  client = new ua::client(
    portnumber,
    hostname_std,
    timeout);

  auto client_log_callback = gcnew Action<UA::LogLevel, UA::LogCategory, String^>(this, &UA::Client::LogCallback);
  auto client_log_adapter = gcnew UA::LogCallbackAdapter(client_log_callback);
  client->add_log_callback(client_log_adapter->NativeLogCallback);
}

UA::Client::Client() :
  disposed(false)
{
  client = new ua::client();

  auto client_log_callback = gcnew Action<UA::LogLevel, UA::LogCategory, String^>(this, &UA::Client::LogCallback);
  auto client_log_adapter = gcnew UA::LogCallbackAdapter(client_log_callback);
  client->add_log_callback(client_log_adapter->NativeLogCallback);
}

UA::Client::~Client()
{
  if (disposed)
    return;

  this->!Client();

  disposed = true;
}

UA::Client::!Client()
{
  if (client != nullptr)
  {
    client->disconnect();
    delete client;
    client = nullptr;
  }
}

void UA::Client::Connect()
{
  if (disposed || client == nullptr)
  {
    throw gcnew ObjectDisposedException(nameof(Client));
  }

  client->connect();
}

void UA::Client::Disconnect()
{
  if (disposed || client == nullptr)
  {
    throw gcnew ObjectDisposedException(nameof(Client));
  }

  client->disconnect();
}

generic<class T>
T UA::Client::Get(
  String^ name,
  array<String^>^ path)
{
  if (disposed || client == nullptr)
  {
    throw gcnew ObjectDisposedException(nameof(Client));
  }

  const auto name_std = UA::Convert::ToStdString(name);
  const auto path_std = UA::Convert::ToStdStringVector(path);

  auto adapter = gcnew UA::GenericClientVariableGetterCallbackAdapter<T>();

  client->get(
    name_std,
    path_std,
    adapter->NativeGetterCallback);

  return adapter->Value;
}

void UA::Client::Get(
  String^ name,
  array<String^>^ path,
  Action<UA::Variant^>^ getter)
{
  if (disposed || client == nullptr)
  {
    throw gcnew ObjectDisposedException(nameof(Client));
  }

  const auto name_std = UA::Convert::ToStdString(name);
  const auto path_std = UA::Convert::ToStdStringVector(path);

  auto adapter = gcnew UA::ClientVariableGetterCallbackAdapter(getter);

  client->get(
    name_std,
    path_std,
    adapter->NativeGetterCallback);
}

generic<class T>
void UA::Client::Set(
  String^ name,
  array<String^>^ path,
  T value)
{
  if (disposed || client == nullptr)
  {
    throw gcnew ObjectDisposedException(nameof(Client));
  }

  const auto name_std = UA::Convert::ToStdString(name);
  const auto path_std = UA::Convert::ToStdStringVector(path);

  auto adapter = gcnew UA::GenericClientVariableSetterCallbackAdapter<T>(value);

  client->set(
    name_std,
    path_std,
    adapter->NativeSetterCallback);
}

void UA::Client::Set(
  String^ name,
  array<String^>^ path,
  Action<UA::Variant^>^ setter)
{
  if (disposed || client == nullptr)
  {
    throw gcnew ObjectDisposedException(nameof(Client));
  }

  const auto name_std = UA::Convert::ToStdString(name);
  const auto path_std = UA::Convert::ToStdStringVector(path);

  auto adapter = gcnew UA::ClientVariableSetterCallbackAdapter(setter);

  client->set(
    name_std,
    path_std,
    adapter->NativeSetterCallback);
}

void UA::Client::Call(
  String^ name,
  array<String^>^ path,
  Action<UA::Variant^>^ request,
  Action<UA::Variant^>^ response)
{
  if (disposed || client == nullptr)
  {
    throw gcnew ObjectDisposedException(nameof(Client));
  }

  const auto name_std = UA::Convert::ToStdString(name);
  const auto path_std = UA::Convert::ToStdStringVector(path);

  auto adapter = gcnew UA::ClientMethodCallbackAdapter(request, response);

  client->call(
    name_std,
    path_std,
    adapter->NativeRequestCallback,
    adapter->NativeResponseCallback);
}

void UA::Client::LogCallback(UA::LogLevel level, UA::LogCategory category, String^ message)
{
  try
  {
    OnLogChanged(level, category, message);
  }
  finally
  {
    LogChanged(this, gcnew LogEventArgs(level, category, message));
  }
}
