#include <reopen62541/client.h>

ua::client::client(const int portnumber, const std::string& hostname, const int timeout) :
  client_portnumber(portnumber),
  client_hostname(hostname),
  client_url("opc.tcp://" + hostname + ":" + std::to_string(portnumber)),
  client_connected(false)
{
  client_instance = std::shared_ptr<UA_Client>(UA_Client_new(), UA_Client_delete);
  client_config = UA_Client_getConfig(client_instance.get());

  // custom logger
  {
    client_config->logger.log = log_callback_handler;
    client_config->logger.context = this;
    client_config->logger.clear = nullptr;
  }
  
  const auto status = UA_ClientConfig_setDefault(client_config);
  
  if (status != UA_STATUSCODE_GOOD)
  {
    throw std::runtime_error("Unable to bootstrap the OPC UA client instance!");
  }

  // custom timeout
  client_config->timeout = timeout;
}

ua::client::~client()
{
}

int ua::client::portnumber() const
{
  return client_portnumber;
}

const std::string& ua::client::hostname() const
{
  return client_hostname;
}

const std::string& ua::client::url() const
{
  return client_url;
}

bool ua::client::connected() const
{
  return client_connected;
}

void ua::client::connect()
{
  if (client_connected)
  {
    throw std::runtime_error("client is already connected!");
  }

  UA_StatusCode status;

  try
  {
    status = UA_Client_connect(client_instance.get(), client_url.c_str());

    client_connected = (status == UA_STATUSCODE_GOOD);
  }
  catch (const std::exception& exception)
  {
    throw exception;
  }
  catch (...)
  {
    throw std::runtime_error("Don't know the exception! :~(");
  }

  if (status != UA_STATUSCODE_GOOD)
  {
    throw std::runtime_error(UA_StatusCode_name(status));
  }
}

void ua::client::disconnect()
{
  if (client_connected == false)
  {
    return;
  }

  client_connected = false;

  UA_StatusCode status;

  try
  {
    status = UA_Client_disconnect(client_instance.get());
  }
  catch (const std::exception& exception)
  {
    throw exception;
  }
  catch (...)
  {
    throw std::runtime_error("Don't know the exception! :~(");
  }

  if (status != UA_STATUSCODE_GOOD)
  {
    throw std::runtime_error(UA_StatusCode_name(status));
  }
}

void ua::client::add_log_callback(
  std::function<ua::client_log_callback> callback)
{
  client_log_callbacks.push_back(callback);
}

void ua::client::call(
  const std::string& name,
  const std::vector<std::string>& path,
  std::function<ua::client_method_request_callback> request,
  std::function<ua::client_method_response_callback> response)
{
  size_t ninputs = 0, noutputs = 0;

  get_method_nargs(name, path, &ninputs, &noutputs);

  std::vector<UA_Variant> inputs(ninputs);
  std::for_each(inputs.begin(), inputs.end(),
    [](UA_Variant& input) { UA_Variant_init(&input); });

  ua::output input(inputs.data(), inputs.size());
  request(input);

  UA_Variant* outputs;
  UA_StatusCode status;

  try
  {
    const auto node = UID(path, name);
    const auto parent = UID(path);

    const auto node_id = UA_NODEID_STRING(NS, STRINGS(node));
    const auto parent_id = UA_NODEID_STRING(NS, STRINGS(parent));

    status = UA_Client_call(
      client_instance.get(),
      parent_id,
      node_id,
      inputs.size(),
      inputs.data(),
      &noutputs,
      &outputs);
  }
  catch (const std::exception& exception)
  {
    throw exception;
  }
  catch (...)
  {
    throw std::runtime_error("Don't know the exception! :~(");
  }

  if (status != UA_STATUSCODE_GOOD)
  {
    throw ua::client_error(status);
  }

  ua::input output(outputs, noutputs);
  response(output);

  UA_Array_delete(outputs, noutputs, &UA_TYPES[UA_TYPES_VARIANT]);
}

void ua::client::log_callback_handler(
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

  for (const auto& log : static_cast<ua::client*>(context)->client_log_callbacks)
  {
    log(level, category, message);
  }
}

void ua::client::get_method_nargs(
  const std::string& name,
  const std::vector<std::string>& path,
  size_t* number_of_inputs,
  size_t* number_of_outputs)
{
  const auto node = UID(path, name);
  const auto node_id = UA_NODEID_STRING_ALLOC(NS, STRINGS(node)); // ALLOC

  UA_BrowseRequest request;
  {
    UA_BrowseRequest_init(&request);

    request.requestedMaxReferencesPerNode = 0;
    request.nodesToBrowse = UA_BrowseDescription_new();
    request.nodesToBrowseSize = 1;
    request.nodesToBrowse[0].nodeId = node_id;
    request.nodesToBrowse[0].resultMask = UA_BROWSERESULTMASK_BROWSENAME;
  }

  UA_BrowseResponse response = UA_Client_Service_browse(client_instance.get(), request);

  for (size_t i = 0; i < response.resultsSize; ++i)
  {
    for (size_t j = 0; j < response.results[i].referencesSize; ++j)
    {
      const auto nodeName = ua::convert::to_string(response.results[i].references[j].browseName.name);
      const auto nodeId = response.results[i].references[j].nodeId.nodeId;

      if (nodeName == "InputArguments")
      {
        UA_Variant variant;
        UA_Variant_init(&variant);
        const UA_StatusCode status = UA_Client_readValueAttribute(client_instance.get(), nodeId, &variant);
        const size_t value = variant.arrayLength;
        UA_Variant_clear(&variant);

        if (status != UA_STATUSCODE_GOOD)
        {
          throw std::runtime_error(UA_StatusCode_name(status));
        }

        *number_of_inputs = value;
      }

      if (nodeName == "OutputArguments")
      {
        UA_Variant variant;
        UA_Variant_init(&variant);
        const UA_StatusCode status = UA_Client_readValueAttribute(client_instance.get(), nodeId, &variant);
        const size_t value = variant.arrayLength;
        UA_Variant_clear(&variant);

        if (status != UA_STATUSCODE_GOOD)
        {
          throw std::runtime_error(UA_StatusCode_name(status));
        }

        *number_of_outputs = value;
      }
    }
  }

  UA_BrowseRequest_clear(&request);
  UA_BrowseResponse_clear(&response);
}
