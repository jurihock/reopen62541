#pragma once

#include <reopen62541/convert.h>
#include <reopen62541/error.h>
#include <reopen62541/input.h>
#include <reopen62541/output.h>
#include <reopen62541/strings.h>
#include <reopen62541/uid.h>

#include <open62541.h>

#include <algorithm>
#include <memory>
#include <string>
#include <vector>

namespace ua
{
  typedef void (client_method_request_callback)(ua::output& request);
  typedef void (client_method_response_callback)(const ua::input& response);

  class client
  {
  public:

    client(const std::string& url);
    client(const std::string& url, int timeout);
    ~client();

    const std::string& url() const;

    bool connected() const;

    void connect();
    void disconnect();

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

    const std::string client_url;
    volatile UA_Boolean client_connected;

    void get_method_nargs(const std::string& name, const std::vector<std::string>& path, size_t* number_of_inputs, size_t* number_of_outputs);
  };
}
