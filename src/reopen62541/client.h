#pragma once

#include <reopen62541/convert.h>
#include <reopen62541/exception.h>
#include <reopen62541/strings.h>
#include <reopen62541/uid.h>
#include <reopen62541/variant.h>

#include <open62541.h>

#include <cstdarg>
#include <functional>
#include <memory>
#include <string>
#include <vector>

namespace ua
{
  typedef void (client_log_callback)(UA_LogLevel level, UA_LogCategory category, const std::string& message);
  typedef std::vector<std::function<ua::client_log_callback>> client_log_callback_vector;

  typedef void (client_variable_getter_callback)(const ua::variant& output);
  typedef void (client_variable_setter_callback)(ua::variant& input);

  typedef void (client_method_request_callback)(ua::variant& input);
  typedef void (client_method_response_callback)(const ua::variant& output);

  /**
   * Represents an UA_Client instance.
   */
  class client
  {
  public:

    client(const int portnumber = 4840, const std::string& hostname = "localhost", const int timeout = 5000);
    ~client();

    int portnumber() const;
    const std::string& hostname() const;
    const std::string& url() const;

    bool connected() const;

    void connect();
    void disconnect();

    void add_log_callback(
      std::function<ua::client_log_callback> callback);

    void get(
      const std::string& name,
      const std::vector<std::string>& path,
      std::function<ua::client_variable_getter_callback> getter);

    void set(
      const std::string& name,
      const std::vector<std::string>& path,
      std::function<ua::client_variable_setter_callback> setter);

    void call(
      const std::string& name,
      const std::vector<std::string>& path,
      std::function<ua::client_method_request_callback> request,
      std::function<ua::client_method_response_callback> response);

  protected:

    static inline int NS = 0;
    static inline char LC[] = "en";
    ua::strings STRINGS;
    ua::uid UID;

    std::shared_ptr<UA_Client> client_instance;
    UA_ClientConfig* client_config;

  private:

    const int client_portnumber;
    const std::string client_hostname;
    const std::string client_url;
    volatile UA_Boolean client_connected;

    ua::client_log_callback_vector client_log_callbacks;

    static void log_callback_handler(
      void* context,
      UA_LogLevel level,
      UA_LogCategory category,
      const char* format,
      va_list args);

    void get_method_nargs(
      const std::string& name,
      const std::vector<std::string>& path,
      size_t* number_of_inputs,
      size_t* number_of_outputs);
  };
}
