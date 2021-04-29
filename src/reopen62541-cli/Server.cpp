#include <reopen62541-cli/Server.h>

#include <reopen62541-cli/Adapters/LogCallbackAdapter.h>
#include <reopen62541-cli/Adapters/GenericServerVariableCallbackAdapter.h>
#include <reopen62541-cli/Adapters/ServerVariableCallbackAdapter.h>
#include <reopen62541-cli/Adapters/ServerMethodCallbackAdapter.h>

UA::Server::Server(int portnumber, String^ hostname, String^ name, String^ uri) :
  disposed(false),
  server(nullptr)
{
  const auto hostname_std = UA::Convert::ToStdString(hostname);
  const auto name_std = UA::Convert::ToStdString(name);
  const auto uri_std = UA::Convert::ToStdString(uri);

  server = new ua::server(
    portnumber,
    hostname_std,
    name_std,
    uri_std);

  auto server_log_callback = gcnew Action<UA::LogLevel, UA::LogCategory, String^>(this, &UA::Server::LogCallback);
  auto server_log_adapter = gcnew UA::LogCallbackAdapter(server_log_callback);
  server->add_log_callback(server_log_adapter->NativeLogCallback);
}

UA::Server::Server() :
  disposed(false),
  server(nullptr)
{
  server = new ua::server();

  auto server_log_callback = gcnew Action<UA::LogLevel, UA::LogCategory, String^>(this, &UA::Server::LogCallback);
  auto server_log_adapter = gcnew UA::LogCallbackAdapter(server_log_callback);
  server->add_log_callback(server_log_adapter->NativeLogCallback);
}

UA::Server::~Server()
{
  if (disposed)
    return;

  this->!Server();

  disposed = true;
}

UA::Server::!Server()
{
  if (server != nullptr)
  {
    server->shutdown();
    delete server;
    server = nullptr;
  }
}

void UA::Server::Run()
{
  if (disposed || server == nullptr)
  {
    throw gcnew ObjectDisposedException(nameof(Server));
  }

  server->run();
}

void UA::Server::Shutdown()
{
  if (disposed || server == nullptr)
  {
    throw gcnew ObjectDisposedException(nameof(Server));
  }

  server->shutdown();
}

void UA::Server::AddFolder(
  String^ name,
  String^ description,
  ... array<String^>^ path)
{
  if (disposed || server == nullptr)
  {
    throw gcnew ObjectDisposedException(nameof(Server));
  }

  const auto name_std = UA::Convert::ToStdString(name);
  const auto description_std = UA::Convert::ToStdString(description);
  const auto path_std = UA::Convert::ToStdStringVector(path);

  server->add_folder(
    name_std,
    description_std,
    path_std);
}

void UA::Server::AddObject(
  String^ name,
  String^ description,
  ... array<String^>^ path)
{
  if (disposed || server == nullptr)
  {
    throw gcnew ObjectDisposedException(nameof(Server));
  }

  const auto name_std = UA::Convert::ToStdString(name);
  const auto description_std = UA::Convert::ToStdString(description);
  const auto path_std = UA::Convert::ToStdStringVector(path);

  server->add_object(
    name_std,
    description_std,
    path_std);
}

generic<class T>
void UA::Server::AddVariable(
  String^ name,
  String^ description,
  array<String^>^ path,
  Func<T>^ getter,
  Action<T>^ setter)
{
  if (disposed || server == nullptr)
  {
    throw gcnew ObjectDisposedException(nameof(Server));
  }

  const auto name_std = UA::Convert::ToStdString(name);
  const auto description_std = UA::Convert::ToStdString(description);
  const auto path_std = UA::Convert::ToStdStringVector(path);

  auto datatype = UA::Convert::ToUaDataType<T>();
  auto valuerank = UA::Convert::ToUaValueRank<T>();
  auto adapter = gcnew UA::GenericServerVariableCallbackAdapter<T>(getter, setter);

  server->add_variable(
    name_std,
    description_std,
    path_std,
    datatype,
    valuerank,
    adapter->NativeGetterCallback,
    adapter->NativeSetterCallback);
}

generic<class T>
void UA::Server::AddVariable(
  String^ name,
  String^ description,
  array<String^>^ path,
  Action<UA::Variant^>^ getter,
  Action<UA::Variant^>^ setter)
{
  if (disposed || server == nullptr)
  {
    throw gcnew ObjectDisposedException(nameof(Server));
  }

  const auto name_std = UA::Convert::ToStdString(name);
  const auto description_std = UA::Convert::ToStdString(description);
  const auto path_std = UA::Convert::ToStdStringVector(path);

  auto datatype = UA::Convert::ToUaDataType<T>();
  auto valuerank = UA::Convert::ToUaValueRank<T>();
  auto adapter = gcnew UA::ServerVariableCallbackAdapter(getter, setter);

  server->add_variable(
    name_std,
    description_std,
    path_std,
    datatype,
    valuerank,
    adapter->NativeGetterCallback,
    adapter->NativeSetterCallback);
}

void UA::Server::AddMethod(
  String^ name,
  String^ description,
  array<String^>^ path,
  array<UA::Argument^>^ inputs,
  array<UA::Argument^>^ outputs,
  Action<UA::Variant^, UA::Variant^>^ action)
{
  if (disposed || server == nullptr)
  {
    throw gcnew ObjectDisposedException(nameof(Server));
  }

  const auto name_std = UA::Convert::ToStdString(name);
  const auto description_std = UA::Convert::ToStdString(description);
  const auto path_std = UA::Convert::ToStdStringVector(path);

  std::vector<ua::argument> inputs_std;

  if (inputs != nullptr)
  {
    for (long i = 0; i < inputs->LongLength; ++i)
    {
      inputs_std.push_back(inputs[i]->ToUaArgument());
    }
  }

  std::vector<ua::argument> outputs_std;

  if (outputs != nullptr)
  {
    for (long i = 0; i < outputs->LongLength; ++i)
    {
      outputs_std.push_back(outputs[i]->ToUaArgument());
    }
  }

  auto adapter = gcnew UA::ServerMethodCallbackAdapter(action);

  server->add_method(
    name_std,
    description_std,
    path_std,
    inputs_std,
    outputs_std,
    adapter->NativeActionCallback);
}

void UA::Server::LogCallback(UA::LogLevel level, UA::LogCategory category, String^ message)
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
