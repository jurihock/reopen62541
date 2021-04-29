#pragma once

#include <reopen62541-cli/Convert.h>

#include <reopen62541/exception.h>

#using <System.dll>

using namespace System;

namespace UA
{
  public ref class StatusCodeException : Exception
  {
  public:

    property UA_StatusCode StatusCode
    {
      UA_StatusCode get() { return ua_status_code; }
    }

    StatusCodeException(UA_StatusCode code, String^ message) :
      Exception(message),
      ua_status_code(code)
    {
    }

    StatusCodeException(UA_StatusCode code, String^ message, Exception^ innerException) :
      Exception(message, innerException),
      ua_status_code(code)
    {
    }

  internal:

    StatusCodeException(const ua::status_code_error& error) :
      Exception(Convert::ToString(error.name())),
      ua_status_code(error.code())
    {
    }

  private:

    UA_StatusCode ua_status_code;
  };

  public ref class ClientException : UA::StatusCodeException
  {
  public:

    ClientException(UA_StatusCode code, String^ message) :
      UA::StatusCodeException(code, message)
    {
    }

    ClientException(UA_StatusCode code, String^ message, Exception^ innerException) :
      UA::StatusCodeException(code, message, innerException)
    {
    }

  internal:

    ClientException(const ua::status_code_error& error) :
      UA::StatusCodeException(error)
    {
    }
  };

  public ref class ServerException : UA::StatusCodeException
  {
  public:

    ServerException(UA_StatusCode code, String^ message) :
      UA::StatusCodeException(code, message)
    {
    }

    ServerException(UA_StatusCode code, String^ message, Exception^ innerException) :
      UA::StatusCodeException(code, message, innerException)
    {
    }

  internal:

    ServerException(const ua::status_code_error& error) :
      UA::StatusCodeException(error)
    {
    }
  };
}
