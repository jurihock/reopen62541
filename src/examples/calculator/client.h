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
    return get<double>("Bill", { "Calculator" });
  }

  void bill(double value) override
  {
    set<double>("Bill", { "Calculator" }, value);
  }

  double tip() override
  {
    return get<double>("Tip", { "Calculator" });
  }

  void tip(double value) override
  {
    set<double>("Tip", { "Calculator" }, value);
  }

  double calculate() override
  {
    double result = 0;

    call("Calculate", { "Calculator" },
      nullptr,
      [&](const ua::variant& output)
      {
        result = output;
      });

    return result;
  }
};
