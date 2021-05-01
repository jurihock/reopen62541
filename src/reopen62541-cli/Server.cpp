#include <reopen62541-cli/Server.h>

#include <reopen62541-cli/Adapters/LogCallbackAdapter.h>
#include <reopen62541-cli/Adapters/GenericServerVariableCallbackAdapter.h>
#include <reopen62541-cli/Adapters/ServerVariableCallbackAdapter.h>
#include <reopen62541-cli/Adapters/ServerMethodCallbackAdapter.h>

UA::Server::Server(ua::server* server) :
  disposed(false),
  server(server)
{
  auto server_log_callback = gcnew Action<UA::LogLevel, UA::LogCategory, String^>(this, &UA::Server::LogCallback);
  auto server_log_adapter = gcnew UA::LogCallbackAdapter(server_log_callback);
  server->add_log_callback(server_log_adapter->NativeLogCallback);
}

UA::Server::Server(int portnumber, String^ hostname, String^ name, String^ uri) :
  UA::Server::Server(new ua::server(portnumber, UA::Convert::ToStdString(hostname), UA::Convert::ToStdString(name), UA::Convert::ToStdString(uri)))
{
}

UA::Server::Server(int portnumber, String^ hostname) :
  UA::Server::Server(new ua::server(portnumber, UA::Convert::ToStdString(hostname)))
{
}

UA::Server::Server(int portnumber) :
  UA::Server::Server(new ua::server(portnumber))
{
}

UA::Server::Server() :
  UA::Server::Server(new ua::server())
{
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
    try
    {
      server->shutdown();
    }
    catch (...)
    {
      // the server could throw an
      // exception on shutdown,
      // so just ignore it in this context
    }
    finally
    {
      delete server;
      server = nullptr;
    }
  }

  if (thread != nullptr)
  {
    try
    {
      if (!thread->Join(1000))
      {
        thread->Abort();
      }
    }
    finally
    {
      thread = nullptr;
    }
  }
}

void UA::Server::RunAsyncPrep()
{
  if (disposed || server == nullptr)
  {
    throw gcnew ObjectDisposedException(nameof(Server));
  }

  try
  {
    server->run_sync_begin();
  }
  catch (const ua::server_error& e)
  {
    throw gcnew UA::ClientException(e);
  }
  catch (const std::exception& e)
  {
    throw gcnew Exception(UA::Convert::ToString(e.what()));
  }
}

void UA::Server::RunAsyncLoop()
{
  if (disposed || server == nullptr)
  {
    throw gcnew ObjectDisposedException(nameof(Server));
  }

  server->run_sync_loop();
}

void UA::Server::RunAsync()
{
  if (disposed || server == nullptr)
  {
    throw gcnew ObjectDisposedException(nameof(Server));
  }

  if (thread != nullptr || server->running())
  {
    return;
  }

  RunAsyncPrep();

  auto loop = gcnew ThreadStart(this, &UA::Server::RunAsyncLoop);

  (thread = gcnew Thread(loop))->Start();
}

void UA::Server::Run()
{
  if (disposed || server == nullptr)
  {
    throw gcnew ObjectDisposedException(nameof(Server));
  }

  try
  {
    server->run();
  }
  catch (const ua::server_error& e)
  {
    throw gcnew UA::ServerException(e);
  }
  catch (const std::exception& e)
  {
    throw gcnew Exception(UA::Convert::ToString(e.what()));
  }
}

void UA::Server::Shutdown()
{
  if (disposed || server == nullptr)
  {
    throw gcnew ObjectDisposedException(nameof(Server));
  }

  try
  {
    server->shutdown();
  }
  catch (const ua::server_error& e)
  {
    throw gcnew UA::ServerException(e);
  }
  catch (const std::exception& e)
  {
    throw gcnew Exception(UA::Convert::ToString(e.what()));
  }
  finally
  {
    if (thread != nullptr)
    {
      if (!thread->Join(1000))
      {
        thread->Abort();
      }

      thread = nullptr;
    }
  }
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

  try
  {
    server->add_folder(
      name_std,
      description_std,
      path_std);
  }
  catch (const ua::server_error& e)
  {
    throw gcnew UA::ServerException(e);
  }
  catch (const std::exception& e)
  {
    throw gcnew Exception(UA::Convert::ToString(e.what()));
  }
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

  try
  {
    server->add_object(
      name_std,
      description_std,
      path_std);
  }
  catch (const ua::server_error& e)
  {
    throw gcnew UA::ServerException(e);
  }
  catch (const std::exception& e)
  {
    throw gcnew Exception(UA::Convert::ToString(e.what()));
  }
}

generic<class T>
void UA::Server::AddVariable(
  String^ name,
  String^ description,
  array<String^>^ path,
  Func<T>^ getter)
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
  auto adapter = gcnew UA::GenericServerVariableCallbackAdapter<T>(getter);

  try
  {
    server->add_variable(
      name_std,
      description_std,
      path_std,
      datatype,
      valuerank,
      adapter->NativeGetterCallback);
  }
  catch (const ua::server_error& e)
  {
    throw gcnew UA::ServerException(e);
  }
  catch (const std::exception& e)
  {
    throw gcnew Exception(UA::Convert::ToString(e.what()));
  }
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

  try
  {
    server->add_variable(
      name_std,
      description_std,
      path_std,
      datatype,
      valuerank,
      adapter->NativeGetterCallback,
      adapter->NativeSetterCallback);
  }
  catch (const ua::server_error& e)
  {
    throw gcnew UA::ServerException(e);
  }
  catch (const std::exception& e)
  {
    throw gcnew Exception(UA::Convert::ToString(e.what()));
  }
}

generic<class T>
void UA::Server::AddVariable(
  String^ name,
  String^ description,
  array<String^>^ path,
  Action<UA::Variant^>^ getter)
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
  auto adapter = gcnew UA::ServerVariableCallbackAdapter(getter);

  try
  {
    server->add_variable(
      name_std,
      description_std,
      path_std,
      datatype,
      valuerank,
      adapter->NativeGetterCallback,
      adapter->NativeSetterCallback);
  }
  catch (const ua::server_error& e)
  {
    throw gcnew UA::ServerException(e);
  }
  catch (const std::exception& e)
  {
    throw gcnew Exception(UA::Convert::ToString(e.what()));
  }
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

  try
  {
    server->add_variable(
      name_std,
      description_std,
      path_std,
      datatype,
      valuerank,
      adapter->NativeGetterCallback,
      adapter->NativeSetterCallback);
  }
  catch (const ua::server_error& e)
  {
    throw gcnew UA::ServerException(e);
  }
  catch (const std::exception& e)
  {
    throw gcnew Exception(UA::Convert::ToString(e.what()));
  }
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

  try
  {
    server->add_method(
      name_std,
      description_std,
      path_std,
      inputs_std,
      outputs_std,
      adapter->NativeActionCallback);
  }
  catch (const ua::server_error& e)
  {
    throw gcnew UA::ServerException(e);
  }
  catch (const std::exception& e)
  {
    throw gcnew Exception(UA::Convert::ToString(e.what()));
  }
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
