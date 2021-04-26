#pragma once

#include <reopen62541/argument.h>
#include <reopen62541/convert.h>
#include <reopen62541/error.h>
#include <reopen62541/strings.h>
#include <reopen62541/uid.h>
#include <reopen62541/variant.h>

#include <open62541.h>

#include <algorithm>
#include <cstdarg>
#include <cstdio>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <thread>
#include <vector>

namespace ua
{
  typedef void (server_log_callback)(UA_LogLevel level, UA_LogCategory category, const std::string& message);
  typedef std::vector<std::function<ua::server_log_callback>> server_log_callback_vector;

  typedef void (server_property_getter_callback)(ua::variant& output);
  typedef std::map<std::string, std::function<ua::server_property_getter_callback>> server_property_getter_callback_map;

  typedef void (server_property_setter_callback)(const ua::variant& input);
  typedef std::map<std::string, std::function<ua::server_property_setter_callback>> server_property_setter_callback_map;

  typedef void (server_method_callback)(const ua::variant& input, ua::variant& output);
  typedef std::map<std::string, std::function<ua::server_method_callback>> server_method_callback_map;

  class server
  {
  public:

    server(const int portnumber, const std::string& hostname, const std::string& name, const std::string& uri);
    ~server();

    int portnumber() const;
    const std::string& hostname() const;
    const std::string& name() const;
    const std::string& uri() const;

    bool running() const;

    void run_async();
    void run();
    void shutdown();

    void add_log_callback(
      std::function<ua::server_log_callback> callback);

    void add_folder(
      const std::string& name,
      const std::string& description,
      const std::vector<std::string>& path = std::vector<std::string>());

    void add_object(
      const std::string& name,
      const std::string& description,
      const std::vector<std::string>& path = std::vector<std::string>());

    void add_property(
      const std::string& name,
      const std::string& description,
      const std::vector<std::string>& path,
      const uint32_t datatype,
      const int32_t valuerank,
      UA_DataSource callback);

    template<typename T>
    void add_property(
      const std::string& name,
      const std::string& description,
      const std::vector<std::string>& path,
      UA_DataSource callback)
    {
      const uint32_t datatype = ua::convert::to_ua_data_type<T>();
      const int32_t valuerank = ua::convert::to_ua_value_rank<T>();
      
      add_property(name, description, path, datatype, valuerank, callback);
    }

    template<typename T>
    void add_property(
      const std::string& name,
      const std::string& description,
      const std::vector<std::string>& path,
      std::function<ua::server_property_getter_callback> getter)
    {
      const std::string node = ua::convert::to_delimited_string(path, name);

      property_getter_callbacks[node] = getter;
      property_setter_callbacks[node] = nullptr;

      UA_DataSource callback;
      {
        callback.read = property_getter_callback_handler;
        callback.write = nullptr;
      }

      add_property<T>(name, description, path, callback);
    };

    template<typename T>
    void add_property(
      const std::string& name,
      const std::string& description,
      const std::vector<std::string>& path,
      std::function<ua::server_property_getter_callback> getter,
      std::function<ua::server_property_setter_callback> setter)
    {
      const std::string node = UID(path, name);

      server_property_getter_callbacks[node] = getter;
      server_property_setter_callbacks[node] = setter;

      UA_DataSource callback;
      {
        callback.read = property_getter_callback_handler;
        callback.write = property_setter_callback_handler;
      }

      add_property<T>(name, description, path, callback);
    };

    void add_method(
      const std::string& name,
      const std::string& description,
      const std::vector<std::string>& path,
      const std::vector<ua::argument>& inputs,
      const std::vector<ua::argument>& outputs,
      UA_MethodCallback callback);

    void add_method(
      const std::string& name,
      const std::string& description,
      const std::vector<std::string>& path,
      const std::vector<ua::argument>& inputs,
      const std::vector<ua::argument>& outputs,
      std::function<ua::server_method_callback> callback)
    {
      const std::string node = UID(path, name);

      server_method_callbacks[node] = callback;

      add_method(name, description, path, inputs, outputs, method_callback_handler);
    };

    void add_method(
      const std::string& name,
      const std::string& description,
      const std::vector<std::string>& path,
      std::function<void()> callback)
    {
      const std::string node = UID(path, name);

      server_method_callbacks[node] = [callback](const ua::variant&, ua::variant&) { callback(); };

      add_method(name, description, path, {}, {}, method_callback_handler);
    };

  protected:

    static inline int NS = 0;
    static inline char LC[] = "en";
    ua::strings STRINGS;
    ua::uid UID;

    std::shared_ptr<UA_Server> server_instance;
    UA_ServerConfig* server_config;

  private:

    const int server_portnumber;
    const std::string server_hostname;
    const std::string server_name;
    const std::string server_uri;

    volatile UA_Boolean server_running;
    std::shared_ptr<std::thread> server_runner;

    ua::server_log_callback_vector server_log_callbacks;
    ua::server_property_getter_callback_map server_property_getter_callbacks;
    ua::server_property_setter_callback_map server_property_setter_callbacks;
    ua::server_method_callback_map server_method_callbacks;

    static void log_callback_handler(
      void* context,
      UA_LogLevel level,
      UA_LogCategory category,
      const char* format,
      va_list args);

    static UA_StatusCode property_getter_callback_handler(
      UA_Server* server,
      const UA_NodeId* sessionId, void* sessionContext,
      const UA_NodeId* variableId, void* variableContext,
      UA_Boolean includeSourceTimeStamp,
      const UA_NumericRange* range,
      UA_DataValue* value);

    static UA_StatusCode property_setter_callback_handler(
      UA_Server* server,
      const UA_NodeId* sessionId, void* sessionContext,
      const UA_NodeId* variableId, void* variableContext,
      const UA_NumericRange* range,
      const UA_DataValue* value);

    static UA_StatusCode method_callback_handler(
      UA_Server* server,
      const UA_NodeId* sessionId, void* sessionContext,
      const UA_NodeId* methodId, void* methodContext,
      const UA_NodeId* objectId, void* objectContext,
      size_t inputSize, const UA_Variant* input,
      size_t outputSize, UA_Variant* output);
  };
}
