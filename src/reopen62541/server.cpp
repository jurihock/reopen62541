#include <reopen62541/server.h>

ua::server::server(const int portnumber, const std::string& hostname, const std::string& name, const std::string& uri) :
  server_portnumber(portnumber),
  server_hostname(hostname),
  server_name(name),
  server_uri(uri),
  server_running(false),
  server_runner(nullptr)
{
  server_instance = std::shared_ptr<UA_Server>(UA_Server_new(), UA_Server_delete);
  server_config = UA_Server_getConfig(server_instance.get());

  // custom logger
  {
    server_config->logger.log = log_callback_handler;
    server_config->logger.context = this;
    server_config->logger.clear = nullptr;
  }

  const auto status = UA_ServerConfig_setMinimal(server_config, portnumber, nullptr);

  if (status != UA_STATUSCODE_GOOD)
  {
    throw ua::server_error(status);
  }

  // custom configs
  {
    UA_String_deleteMembers(&server_config->customHostname);
    server_config->customHostname = UA_STRING_ALLOC(STRINGS(hostname)); // ALLOC

    UA_String_deleteMembers(&server_config->applicationDescription.applicationName.locale);
    UA_String_deleteMembers(&server_config->applicationDescription.applicationName.text);
    server_config->applicationDescription.applicationName = UA_LOCALIZEDTEXT_ALLOC(LC, STRINGS(name)); // ALLOC

    UA_String_deleteMembers(&server_config->applicationDescription.applicationUri);
    server_config->applicationDescription.applicationUri = UA_STRING_ALLOC(STRINGS(uri)); // ALLOC
  }

  // overwrite endpoint description due to
  // https://github.com/open62541/open62541/issues/1175
  for (size_t i = 0; i < server_config->endpointsSize; ++i)
  {
    UA_String_deleteMembers(&server_config->endpoints[i].server.applicationName.locale);
    UA_String_deleteMembers(&server_config->endpoints[i].server.applicationName.text);
    server_config->endpoints[i].server.applicationName = UA_LOCALIZEDTEXT_ALLOC(LC, STRINGS(name)); // ALLOC

    UA_String_deleteMembers(&server_config->endpoints[i].server.applicationUri);
    server_config->endpoints[i].server.applicationUri = UA_STRING_ALLOC(STRINGS(uri)); // ALLOC
  }

  // check namespace index
  {
    size_t ns;

    const auto status = UA_Server_getNamespaceByName(server_instance.get(), UA_STRING(STRINGS(uri)), &ns);

    if (status != UA_STATUSCODE_GOOD)
    {
      throw ua::server_error(status);
    }

    if (ns != NS1)
    {
      std::stringstream message;
      message << "Unexpected namespace index for the specified application uri \"" << uri << "\"! ";
      message << "Assumed " << NS1 << " got " << ns << ".";
      throw std::runtime_error(message.str());
    }
  }
}

ua::server::~server()
{
  shutdown();
}

int ua::server::portnumber() const
{
  return server_portnumber;
}

const std::string& ua::server::hostname() const
{
  return server_hostname;
}

const std::string& ua::server::name() const
{
  return server_name;
}

const std::string& ua::server::uri() const
{
  return server_uri;
}

bool ua::server::running() const
{
  return server_running;
}

void ua::server::run_sync_begin()
{
  UA_StatusCode status = UA_STATUSCODE_GOOD;

  if (!server_running)
  {
    status = UA_Server_run_startup(server_instance.get());

    server_running = (status == UA_STATUSCODE_GOOD);
  }

  if (status != UA_STATUSCODE_GOOD)
  {
    throw ua::server_error(status);
  }
}

void ua::server::run_sync_end()
{
  UA_StatusCode status = UA_STATUSCODE_GOOD;

  if (server_running)
  {
    server_running = false;

    status = UA_Server_run_shutdown(server_instance.get());
  }

  if (status != UA_STATUSCODE_GOOD)
  {
    throw ua::server_error(status);
  }
}

void ua::server::run_sync_step()
{
  if (server_running)
  {
    UA_Server_run_iterate(server_instance.get(), true);
  }
}

void ua::server::run_sync_loop()
{
  while (server_running)
  {
    UA_Server_run_iterate(server_instance.get(), true);
  }
}

#ifndef __cplusplus_cli
void ua::server::run_async()
{
  if (server_running)
  {
    return;
  }

  if (server_runner != nullptr)
  {
    if (server_runner->joinable())
      server_runner->join();

    server_runner = nullptr;
  }

  run_sync_begin();

  server_runner = std::shared_ptr<std::thread>(
    new std::thread([this]() { run_sync_loop(); }));
}
#endif

void ua::server::run()
{
  if (server_running)
  {
    return;
  }

  run_sync_begin();
  run_sync_loop();
}

void ua::server::shutdown()
{
  run_sync_end();

  if (server_runner != nullptr)
  {
    if (server_runner->joinable())
      server_runner->join();

    server_runner = nullptr;
  }
}

void ua::server::add_log_callback(
  std::function<ua::server_log_callback> callback)
{
  server_log_callbacks.push_back(callback);
};

void ua::server::add_folder(
  const std::string& name,
  const std::string& description,
  const std::vector<std::string>& path)
{
  auto attributes = UA_ObjectAttributes_default;
  {
    attributes.displayName = UA_LOCALIZEDTEXT(LC, STRINGS(name));
    attributes.description = UA_LOCALIZEDTEXT(LC, STRINGS(description));
  }

  const auto node = UID(path, name);
  const auto parent = UID(path);

  const auto node_id = UA_NODEID_STRING(NS1, STRINGS(node));
  const auto node_name = UA_QUALIFIEDNAME(NS1, STRINGS(name));
  const auto node_type = UA_NODEID_NUMERIC(NS0, UA_NS0ID_FOLDERTYPE);

  const auto parent_id = path.empty()
    ? UA_NODEID_NUMERIC(NS0, UA_NS0ID_OBJECTSFOLDER)
    : UA_NODEID_STRING(NS1, STRINGS(parent));
  const auto parent_type = UA_NODEID_NUMERIC(NS0, UA_NS0ID_ORGANIZES);

  const auto status = UA_Server_addObjectNode(
    server_instance.get(),
    node_id,
    parent_id,
    parent_type,
    node_name,
    node_type,
    attributes,
    nullptr,
    nullptr);

  if (status != UA_STATUSCODE_GOOD)
  {
    throw ua::server_error(status);
  }
}

void ua::server::add_object(
  const std::string& name,
  const std::string& description,
  const std::vector<std::string>& path)
{
  auto attributes = UA_ObjectAttributes_default;
  {
    attributes.displayName = UA_LOCALIZEDTEXT(LC, STRINGS(name));
    attributes.description = UA_LOCALIZEDTEXT(LC, STRINGS(description));
  }

  const auto node = UID(path, name);
  const auto parent = UID(path);

  const auto node_id = UA_NODEID_STRING(NS1, STRINGS(node));
  const auto node_name = UA_QUALIFIEDNAME(NS1, STRINGS(name));
  const auto node_type = UA_NODEID_NUMERIC(NS0, UA_NS0ID_BASEOBJECTTYPE);

  const auto parent_id = path.empty()
    ? UA_NODEID_NUMERIC(NS0, UA_NS0ID_OBJECTSFOLDER)
    : UA_NODEID_STRING(NS1, STRINGS(parent));
  const auto parent_type = UA_NODEID_NUMERIC(NS0, UA_NS0ID_ORGANIZES);

  const auto status = UA_Server_addObjectNode(
    server_instance.get(),
    node_id,
    parent_id,
    parent_type,
    node_name,
    node_type,
    attributes,
    nullptr,
    nullptr);

  if (status != UA_STATUSCODE_GOOD)
  {
    throw ua::server_error(status);
  }
}

void ua::server::add_variable(
  const std::string& name,
  const std::string& description,
  const std::vector<std::string>& path,
  const uint32_t datatype,
  const int32_t valuerank,
  UA_DataSource callback)
{
  auto attributes = UA_VariableAttributes_default;
  {
    attributes.displayName = UA_LOCALIZEDTEXT(LC, STRINGS(name));
    attributes.description = UA_LOCALIZEDTEXT(LC, STRINGS(description));

    attributes.dataType = UA_TYPES[datatype].typeId;
    attributes.valueRank = valuerank;

    if (callback.read != nullptr)
    {
      attributes.accessLevel |= UA_ACCESSLEVELMASK_READ;
    }

    if (callback.write != nullptr)
    {
      attributes.accessLevel |= UA_ACCESSLEVELMASK_WRITE;
    }
  }

  const auto node = UID(path, name);
  const auto parent = UID(path);

  const auto node_id = UA_NODEID_STRING(NS1, STRINGS(node));
  const auto node_name = UA_QUALIFIEDNAME(NS1, STRINGS(name));
  const auto node_type = UA_NODEID_NUMERIC(NS0, UA_NS0ID_BASEDATAVARIABLETYPE);

  const auto parent_id = UA_NODEID_STRING(NS1, STRINGS(parent));
  const auto parent_type = UA_NODEID_NUMERIC(NS0, UA_NS0ID_HASCOMPONENT);

  const auto status = UA_Server_addDataSourceVariableNode(
    server_instance.get(),
    node_id,
    parent_id,
    parent_type,
    node_name,
    node_type,
    attributes,
    callback,
    nullptr,
    nullptr);

  if (status != UA_STATUSCODE_GOOD)
  {
    throw ua::server_error(status);
  }
}

void ua::server::add_variable(
  const std::string& name,
  const std::string& description,
  const std::vector<std::string>& path,
  const uint32_t datatype,
  const int32_t valuerank,
  std::function<ua::server_variable_getter_callback> getter)
{
  const auto node = UID(path, name);

  server_variable_getter_callbacks[node] = getter;
  server_variable_setter_callbacks[node] = nullptr;

  UA_DataSource callback;
  {
    callback.read = variable_getter_callback_handler;
    callback.write = nullptr;
  }

  add_variable(name, description, path, datatype, valuerank, callback);
}

void ua::server::add_variable(
  const std::string& name,
  const std::string& description,
  const std::vector<std::string>& path,
  const uint32_t datatype,
  const int32_t valuerank,
  std::function<ua::server_variable_getter_callback> getter,
  std::function<ua::server_variable_setter_callback> setter)
{
  const auto node = UID(path, name);

  server_variable_getter_callbacks[node] = getter;
  server_variable_setter_callbacks[node] = setter;

  UA_DataSource callback;
  {
    callback.read = variable_getter_callback_handler;
    callback.write = variable_setter_callback_handler;
  }

  add_variable(name, description, path, datatype, valuerank, callback);
}

void ua::server::add_method(
  const std::string& name,
  const std::string& description,
  const std::vector<std::string>& path,
  const std::vector<ua::argument>& inputs,
  const std::vector<ua::argument>& outputs,
  UA_MethodCallback callback)
{
  auto attributes = UA_MethodAttributes_default;
  {
    attributes.displayName = UA_LOCALIZEDTEXT(LC, STRINGS(name));
    attributes.description = UA_LOCALIZEDTEXT(LC, STRINGS(description));

    attributes.executable = true;
    attributes.userExecutable = true;
  }

  std::vector<UA_Argument> inputArguments(inputs.size());
  for (size_t i = 0; i < inputs.size(); ++i)
    inputs[i].to_ua_argument(LC, STRINGS, inputArguments[i]);

  std::vector<UA_Argument> outputArguments(outputs.size());
  for (size_t i = 0; i < outputs.size(); ++i)
    outputs[i].to_ua_argument(LC, STRINGS, outputArguments[i]);

  const auto node = UID(path, name);
  const auto parent = UID(path);

  const auto node_id = UA_NODEID_STRING(NS1, STRINGS(node));
  const auto node_name = UA_QUALIFIEDNAME(NS1, STRINGS(name));

  const auto parent_id = UA_NODEID_STRING(NS1, STRINGS(parent));
  const auto parent_type = UA_NODEID_NUMERIC(NS0, UA_NS0ID_HASCOMPONENT);

  const auto status = UA_Server_addMethodNode(
    server_instance.get(),
    node_id,
    parent_id,
    parent_type,
    node_name,
    attributes,
    callback,
    inputArguments.size(),
    inputArguments.data(),
    outputArguments.size(),
    outputArguments.data(),
    nullptr,
    nullptr);

  if (status != UA_STATUSCODE_GOOD)
  {
    throw ua::server_error(status);
  }
}

void ua::server::add_method(
  const std::string& name,
  const std::string& description,
  const std::vector<std::string>& path,
  const std::vector<ua::argument>& inputs,
  const std::vector<ua::argument>& outputs,
  std::function<ua::server_method_callback> callback)
{
  const auto node = UID(path, name);

  server_method_callbacks[node] = callback;

  add_method(name, description, path, inputs, outputs, method_callback_handler);
}

void ua::server::add_method(
  const std::string& name,
  const std::string& description,
  const std::vector<std::string>& path,
  std::function<void()> callback)
{
  const auto node = UID(path, name);

  server_method_callbacks[node] = [callback](const ua::variant&, ua::variant&) { callback(); };

  add_method(name, description, path, {}, {}, method_callback_handler);
}

void ua::server::log_callback_handler(
  void* context,
  UA_LogLevel level,
  UA_LogCategory category,
  const char* format,
  va_list args)
{
  if (context == nullptr)
  {
    return;
  }

  std::va_list argscopy;
  va_copy(argscopy, args);

  const auto length = std::vsnprintf(nullptr, 0, format, argscopy) + 1; // incl. \0

  std::vector<char> buffer(length, 0);
  std::vsnprintf(buffer.data(), length, format, args);

  const std::string message(buffer.begin(), buffer.end());

  for (const auto& log : static_cast<ua::server*>(context)->server_log_callbacks)
  {
    log(level, category, message);
  }
}

UA_StatusCode ua::server::variable_getter_callback_handler(
  UA_Server* server,
  const UA_NodeId* sessionId, void* sessionContext,
  const UA_NodeId* variableId, void* variableContext,
  UA_Boolean includeSourceTimeStamp,
  const UA_NumericRange* range,
  UA_DataValue* value)
{
  if (UA_Server_getConfig(server)->logger.context == nullptr)
  {
    return UA_STATUSCODE_BADUNEXPECTEDERROR;
  }

  const auto context = static_cast<ua::server*>(UA_Server_getConfig(server)->logger.context);
  const auto node = ua::convert::to_string(variableId->identifier.string);

  try
  {
    const auto& callback = context->server_variable_getter_callbacks[node];

    ua::variant callback_variant(&value->value, 1);

    callback(callback_variant);

    value->hasValue = true; // WTF?

    return UA_STATUSCODE_GOOD;
  }
  catch (const std::exception& exception)
  {
    const std::string message = "Exception in variable getter " + node + ": " + exception.what();

    for (const auto& log : context->server_log_callbacks)
    {
      log(UA_LOGLEVEL_ERROR, UA_LOGCATEGORY_SERVER, message);
    }

    return UA_STATUSCODE_BADINTERNALERROR;
  }
  catch (...)
  {
    const std::string message = "Unknown exception in variable getter " + node + "!";

    for (const auto& log : context->server_log_callbacks)
    {
      log(UA_LOGLEVEL_ERROR, UA_LOGCATEGORY_SERVER, message);
    }

    return UA_STATUSCODE_BADINTERNALERROR;
  }

  return UA_STATUSCODE_BADINTERNALERROR;
}

UA_StatusCode ua::server::variable_setter_callback_handler(
  UA_Server* server,
  const UA_NodeId* sessionId, void* sessionContext,
  const UA_NodeId* variableId, void* variableContext,
  const UA_NumericRange* range,
  const UA_DataValue* value)
{
  if (UA_Server_getConfig(server)->logger.context == nullptr)
  {
    return UA_STATUSCODE_BADUNEXPECTEDERROR;
  }

  const auto context = static_cast<ua::server*>(UA_Server_getConfig(server)->logger.context);
  const auto node = ua::convert::to_string(variableId->identifier.string);

  try
  {
    const auto& callback = context->server_variable_setter_callbacks[node];

    ua::variant callback_variant(&value->value, 1);

    callback(callback_variant);

    return UA_STATUSCODE_GOOD;
  }
  catch (const std::exception& exception)
  {
    const std::string message = "Exception in variable setter " + node + ": " + exception.what();

    for (const auto& log : context->server_log_callbacks)
    {
      log(UA_LOGLEVEL_ERROR, UA_LOGCATEGORY_SERVER, message);
    }

    return UA_STATUSCODE_BADINTERNALERROR;
  }
  catch (...)
  {
    const std::string message = "Unknown exception in variable setter " + node + "!";

    for (const auto& log : context->server_log_callbacks)
    {
      log(UA_LOGLEVEL_ERROR, UA_LOGCATEGORY_SERVER, message);
    }

    return UA_STATUSCODE_BADINTERNALERROR;
  }

  return UA_STATUSCODE_BADINTERNALERROR;
}

UA_StatusCode ua::server::method_callback_handler(
  UA_Server* server,
  const UA_NodeId* sessionId, void* sessionContext,
  const UA_NodeId* methodId, void* methodContext,
  const UA_NodeId* objectId, void* objectContext,
  size_t inputSize, const UA_Variant* input,
  size_t outputSize, UA_Variant* output)
{
  if (UA_Server_getConfig(server)->logger.context == nullptr)
  {
    return UA_STATUSCODE_BADUNEXPECTEDERROR;
  }

  const auto context = static_cast<ua::server*>(UA_Server_getConfig(server)->logger.context);
  const auto node = ua::convert::to_string(methodId->identifier.string);

  try
  {
    const auto& callback = context->server_method_callbacks[node];

    ua::variant callback_variant_left(input, inputSize);
    ua::variant callback_variant_right(output, outputSize);

    callback(callback_variant_left, callback_variant_right);

    return UA_STATUSCODE_GOOD;
  }
  catch (const std::exception& exception)
  {
    const std::string message = "Exception in method " + node + ": " + exception.what();

    for (const auto& log : context->server_log_callbacks)
    {
      log(UA_LOGLEVEL_ERROR, UA_LOGCATEGORY_SERVER, message);
    }

    return UA_STATUSCODE_BADINTERNALERROR;
  }
  catch (...)
  {
    const std::string message = "Unknown exception in method " + node + "!";

    for (const auto& log : context->server_log_callbacks)
    {
      log(UA_LOGLEVEL_ERROR, UA_LOGCATEGORY_SERVER, message);
    }

    return UA_STATUSCODE_BADINTERNALERROR;
  }

  return UA_STATUSCODE_BADINTERNALERROR;
}
