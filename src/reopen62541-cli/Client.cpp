#include <reopen62541-cli/Client.h>

#include <reopen62541-cli/Adapters/LogCallbackAdapter.h>
#include <reopen62541-cli/Adapters/GenericClientVariableGetterCallbackAdapter.h>
#include <reopen62541-cli/Adapters/GenericClientVariableSetterCallbackAdapter.h>
#include <reopen62541-cli/Adapters/ClientVariableGetterCallbackAdapter.h>
#include <reopen62541-cli/Adapters/ClientVariableSetterCallbackAdapter.h>
#include <reopen62541-cli/Adapters/ClientMethodCallbackAdapter.h>

UA::Client::Client(ua::client* client) :
  disposed(false),
  client(client)
{
  auto client_log_callback = gcnew Action<UA::LogLevel, UA::LogCategory, String^>(this, &UA::Client::LogCallback);
  auto client_log_adapter = gcnew UA::LogCallbackAdapter(client_log_callback);
  client->add_log_callback(client_log_adapter->NativeLogCallback);
}

UA::Client::Client(int portnumber, String^ hostname, int timeout) :
  UA::Client::Client(new ua::client(portnumber, UA::Convert::ToStdString(hostname), timeout))
{
}

UA::Client::Client(int portnumber, String^ hostname) :
  UA::Client::Client(new ua::client(portnumber, UA::Convert::ToStdString(hostname)))
{
}

UA::Client::Client(int portnumber) :
  UA::Client::Client(new ua::client(portnumber))
{
}

UA::Client::Client() :
  UA::Client::Client(new ua::client())
{
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
    try
    {
      client->disconnect();
    }
    catch (...)
    {
      // the client could throw an
      // exception on disconnect,
      // so just ignore it ar this point
    }
    finally
    {
      delete client;
      client = nullptr;
    }
  }
}

void UA::Client::Connect()
{
  if (disposed || client == nullptr)
  {
    throw gcnew ObjectDisposedException(nameof(Client));
  }

  try
  {
    client->connect();
  }
  catch (const ua::client_error& e)
  {
    throw gcnew UA::ClientException(e);
  }
  catch (const std::exception& e)
  {
    throw gcnew Exception(UA::Convert::ToString(e.what()));
  }
}

void UA::Client::Disconnect()
{
  if (disposed || client == nullptr)
  {
    throw gcnew ObjectDisposedException(nameof(Client));
  }

  try
  {
    client->disconnect();
  }
  catch (const ua::client_error& e)
  {
    throw gcnew UA::ClientException(e);
  }
  catch (const std::exception& e)
  {
    throw gcnew Exception(UA::Convert::ToString(e.what()));
  }
}

void UA::Client::Sync(int timeout)
{
  if (disposed || client == nullptr)
  {
    throw gcnew ObjectDisposedException(nameof(Client));
  }

  try
  {
    client->sync(timeout);
  }
  catch (const ua::client_error& e)
  {
    throw gcnew UA::ClientException(e);
  }
  catch (const std::exception& e)
  {
    throw gcnew Exception(UA::Convert::ToString(e.what()));
  }
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
