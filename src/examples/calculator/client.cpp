#include <reopen62541/client.h>

#include <condition_variable>
#include <csignal>
#include <iomanip>
#include <iostream>
#include <map>
#include <mutex>
#include <string>

int main()
{
  Sleep(1000);

  ua::client client("opc.tcp://localhost:4840");

  client.add_log_callback([](UA_LogLevel level, UA_LogCategory category, const std::string& message)
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

    std::cout
      << "CLIENT "
      << "[" << levels.at(level)
      << " " << categories.at(category)
      << "] " << message
      << std::endl;
  });

  client.connect();

  client.call(
    "Calculate",
    { "Calculator" },
    [](ua::output& input)
    {
    },
    [](const ua::input& output)
    {
      std::cout << "result " << output.get<double>();
    });

  client.disconnect();

  // client._client

  //client.CallMethod(
  //  "foo",
  //  { "bar" },
  //  [this](Input& input)
  //  {
  //    
  //  },
  //  [this](const Output& output)
  //  {
  //    
  //  });

  
}

