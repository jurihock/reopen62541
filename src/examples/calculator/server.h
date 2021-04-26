#pragma once

#include <calculator/implementation.h>

#include <reopen62541/server.h>

#include <iomanip>
#include <iostream>
#include <map>
#include <string>

struct calculator_server : calculator_implementation, ua::server
{
  calculator_server() : calculator_implementation(), ua::server(4840, "localhost", "The Bill Tip Calculator", "urn:calculator")
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

      std::cout << "SERVER "
                << "[" << levels.at(level)
                << " " << categories.at(category)
                << "] " << message
                << std::endl;
    });

    add_object(
      "Calculator",
      "The Bill Tip Calculator");

    add_variable<double>(
      "Bill",
      "The bill amount",
      { "Calculator" },
      [&](ua::variant& output)
      {
        output = bill();
      },
      [&](const ua::variant& input)
      {
        bill(input);

        std::cout << "New bill amount is "
                  << std::quoted(std::to_string(bill()))
                  << std::endl;
      });

    add_variable<double>(
      "Tip",
      "The tip percentage",
      { "Calculator" },
      [&](ua::variant& output)
      {
        output = tip();
      },
      [&](const ua::variant& input)
      {
        tip(input);

        std::cout << "New tip percentage is "
                  << std::quoted(std::to_string(tip()))
                  << std::endl;
      });

    add_method(
      "Calculate",
      "Calculate the bill tip",
      { "Calculator" },
      {},
      { ua::argument::scalar<double>("Result", "The tip amount") },
      [&](const ua::variant& input, ua::variant& output)
      {
        std::cout << "Calculating the bill tip..."
                  << std::endl;

        output = calculate();
      });
  }
};
