#pragma once

#include <open62541.h>

namespace ua
{
  class server_error : public std::exception
  {
  public:

    server_error(UA_StatusCode code) :
      std::exception(UA_StatusCode_name(code))
    {
    }
  };

  class client_error : public std::exception
  {
  public:

    client_error(UA_StatusCode code) :
      std::exception(std::string(UA_StatusCode_name(code)).c_str())
    {
    }
  };
}
