#pragma once

#include <open62541.h>

#include <string>

namespace ua
{
  class server_error : public std::exception
  {
  public:

    server_error(UA_StatusCode code) :
      std::exception(to_string(code).c_str())
    {
    }

  private:

    static std::string to_string(UA_StatusCode code)
    {
      #ifdef UA_ENABLE_STATUSCODE_DESCRIPTIONS
      return UA_StatusCode_name(code);
      #else
      return std::to_string(code);
      #endif
    }
  };

  class client_error : public std::exception
  {
  public:

    client_error(UA_StatusCode code) :
      std::exception(to_string(code).c_str())
    {
    }

  private:

    static std::string to_string(UA_StatusCode code)
    {
      #ifdef UA_ENABLE_STATUSCODE_DESCRIPTIONS
      return UA_StatusCode_name(code);
      #else
      return std::to_string(code);
      #endif
    }
  };
}
