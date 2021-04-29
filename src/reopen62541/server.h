#pragma once

#include <reopen62541/argument.h>
#include <reopen62541/convert.h>
#include <reopen62541/error.h>
#include <reopen62541/strings.h>
#include <reopen62541/uid.h>
#include <reopen62541/variant.h>

#include <open62541.h>

#include <chrono>
#include <cstdarg>
#include <functional>
#include <map>
#include <memory>
#include <string>
#include <vector>

#ifndef __cplusplus_cli
#include <thread>
#endif

namespace ua
{
  typedef void (server_log_callback)(UA_LogLevel level, UA_LogCategory category, const std::string& message);
  typedef std::vector<std::function<ua::server_log_callback>> server_log_callback_vector;

  typedef void (server_variable_getter_callback)(ua::variant& output);
  typedef std::map<std::string, std::function<ua::server_variable_getter_callback>> server_variable_getter_callback_map;

  typedef void (server_variable_setter_callback)(const ua::variant& input);
  typedef std::map<std::string, std::function<ua::server_variable_setter_callback>> server_variable_setter_callback_map;

  typedef void (server_method_callback)(const ua::variant& input, ua::variant& output);
  typedef std::map<std::string, std::function<ua::server_method_callback>> server_method_callback_map;

  /**
   * Represents an UA_Server instance.
   */
  class server
  {
  public:

    server(const int portnumber, const std::string& hostname, const std::string& name, const std::string& uri);
    server();
    ~server();

    int portnumber() const;
    const std::string& hostname() const;
    const std::string& name() const;
    const std::string& uri() const;

    bool running() const;

    #ifndef __cplusplus_cli
    void run_async();
    #endif

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

    void add_variable(
      const std::string& name,
      const std::string& description,
      const std::vector<std::string>& path,
      const uint32_t datatype,
      const int32_t valuerank,
      std::function<ua::server_variable_getter_callback> getter);

    void add_variable(
      const std::string& name,
      const std::string& description,
      const std::vector<std::string>& path,
      const uint32_t datatype,
      const int32_t valuerank,
      std::function<ua::server_variable_getter_callback> getter,
      std::function<ua::server_variable_setter_callback> setter);

    template<typename T>
    void add_variable(
      const std::string& name,
      const std::string& description,
      const std::vector<std::string>& path,
      std::function<ua::server_variable_getter_callback> getter)
    {
      const uint32_t datatype = ua::convert::to_ua_data_type<T>();
      const int32_t valuerank = ua::convert::to_ua_value_rank<T>();

      add_variable(name, description, path, datatype, valuerank, getter);
    };

    template<typename T>
    void add_variable(
      const std::string& name,
      const std::string& description,
      const std::vector<std::string>& path,
      std::function<ua::server_variable_getter_callback> getter,
      std::function<ua::server_variable_setter_callback> setter)
    {
      const uint32_t datatype = ua::convert::to_ua_data_type<T>();
      const int32_t valuerank = ua::convert::to_ua_value_rank<T>();

      add_variable(name, description, path, datatype, valuerank, getter, setter);
    };

    void add_method(
      const std::string& name,
      const std::string& description,
      const std::vector<std::string>& path,
      const std::vector<ua::argument>& inputs,
      const std::vector<ua::argument>& outputs,
      std::function<ua::server_method_callback> callback);

    void add_method(
      const std::string& name,
      const std::string& description,
      const std::vector<std::string>& path,
      std::function<void()> callback);

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

    #ifndef __cplusplus_cli
    std::shared_ptr<std::thread> server_runner;
    #else
    std::shared_ptr<void> server_runner;
    #endif

    ua::server_log_callback_vector server_log_callbacks;
    ua::server_variable_getter_callback_map server_variable_getter_callbacks;
    ua::server_variable_setter_callback_map server_variable_setter_callbacks;
    ua::server_method_callback_map server_method_callbacks;

    void add_variable(
      const std::string& name,
      const std::string& description,
      const std::vector<std::string>& path,
      const uint32_t datatype,
      const int32_t valuerank,
      UA_DataSource callback);

    void add_method(
      const std::string& name,
      const std::string& description,
      const std::vector<std::string>& path,
      const std::vector<ua::argument>& inputs,
      const std::vector<ua::argument>& outputs,
      UA_MethodCallback callback);

    static void log_callback_handler(
      void* context,
      UA_LogLevel level,
      UA_LogCategory category,
      const char* format,
      va_list args);

    static UA_StatusCode variable_getter_callback_handler(
      UA_Server* server,
      const UA_NodeId* sessionId, void* sessionContext,
      const UA_NodeId* variableId, void* variableContext,
      UA_Boolean includeSourceTimeStamp,
      const UA_NumericRange* range,
      UA_DataValue* value);

    static UA_StatusCode variable_setter_callback_handler(
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
