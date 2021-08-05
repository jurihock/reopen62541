#pragma once

#include <open62541.h>

#include <stdexcept>
#include <string>

namespace ua
{
  class status_code_error : public std::exception
  {
  public:

    status_code_error(UA_StatusCode code) :
      std::exception(UA_StatusCode_name(code)),
      status_code(code),
      status_code_name(UA_StatusCode_name(code))
    {
    }

    UA_StatusCode code() const
    {
      return status_code;
    }

    const std::string& name() const
    {
      return status_code_name;
    }

  private:

    const UA_StatusCode status_code;
    const std::string status_code_name;
  };

  class server_error : public ua::status_code_error
  {
  public:

    server_error(UA_StatusCode code) :
      ua::status_code_error(code)
    {
    }
  };

  class client_error : public ua::status_code_error
  {
  public:

    client_error(UA_StatusCode code) :
      ua::status_code_error(code)
    {
    }
  };
}
