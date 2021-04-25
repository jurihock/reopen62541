#pragma once

#include <calculator/interface.h>

#include <reopen62541/client.h>

#include <iomanip>
#include <iostream>
#include <map>
#include <string>

struct calculator_client : calculator_interface, ua::client
{
  calculator_client() : ua::client(4840, "localhost")
  {
    add_log_callback([&](UA_LogLevel level, UA_LogCategory category, const std::string& message)
    {
      const std::map<UA_LogLevel, std::string> levels
      {
        { UA_LOGLEVEL_TRACE,   "trace"   },
        { UA_LOGLEVEL_DEBUG,   "debug"   },
        { UA_LOGLEVEL_INFO,    "info"    },
        { UA_LOGLEVEL_WARNING, "warning" },
        { UA_LOGLEVEL_ERROR,   "error"   },
        { UA_LOGLEVEL_FATAL,   "fatal"   },
      };

      const std::map<UA_LogCategory, std::string> categories
      {
        { UA_LOGCATEGORY_NETWORK,        "network"  },
        { UA_LOGCATEGORY_SECURECHANNEL,  "channel"  },
        { UA_LOGCATEGORY_SESSION,        "session"  },
        { UA_LOGCATEGORY_SERVER,         "server"   },
        { UA_LOGCATEGORY_CLIENT,         "client"   },
        { UA_LOGCATEGORY_USERLAND,       "userland" },
        { UA_LOGCATEGORY_SECURITYPOLICY, "security" },
      };

      std::cout << "CLIENT "
                << "[" << levels.at(level)
                << " " << categories.at(category)
                << "] " << message
                << std::endl;
    });
  }

  double bill() override
  {
    return 0; // TODO
  }

  void bill(double bill) override
  {
    return; // TODO
  }

  double tip() override
  {
    return 0; // TODO
  }

  void tip(double tip) override
  {
    return; // TODO
  }

  double calculate() override
  {
    double result = 0;

    call(
      "Calculate",
      { "Calculator" },
      [&](ua::output& request)
      {
      },
      [&](const ua::input& response)
      {
        result = response.get<double>();
      });

    return result;
  }
};
