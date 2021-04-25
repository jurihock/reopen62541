#include <reopen62541/server.h>

#include <condition_variable>
#include <csignal>
#include <iomanip>
#include <iostream>
#include <map>
#include <mutex>
#include <string>

std::mutex signal_mutex;
std::condition_variable signal_variable;

void onsignal(int code)
{
  signal_variable.notify_all();
}

struct calculator
{
  double bill = 0;
  double tip = 10;

  double calculate()
  {
    return bill * tip / 100.0;
  }
};

struct calculator_server : calculator, ua::server
{
  calculator_server();
};

int main()
{
  std::unique_lock<std::mutex> signal_lock(signal_mutex);

  std::signal(SIGINT, onsignal);
  std::signal(SIGTERM, onsignal);

  calculator_server server;

  server.run_async();

  std::cout << "Waiting for further instructions..." << std::endl;
  signal_variable.wait(signal_lock);
  std::cout << "Shutting down..." << std::endl;

  server.shutdown();

  return 0;
}

calculator_server::calculator_server() : calculator(), ua::server(4840, "localhost", "The Bill Tip calculator", "urn:calculator")
{
  add_log_callback([](UA_LogLevel level, UA_LogCategory category, const std::string& message)
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
      << "SERVER "
      << "[" << levels.at(level)
      << " " << categories.at(category)
      << "] " << message
      << std::endl;
  });

  add_object(
    "Calculator",
    "The Bill Tip calculator");

  add_property<double>(
    "Bill",
    "The bill amount",
    { "Calculator" },
    [this](ua::output& output)
    {
      output << bill;
    },
    [this](const ua::input& input)
    {
      input >> bill;

      std::cout << "New bill amount is "
                << std::quoted(std::to_string(bill))
                << std::endl;
    });

  add_property<double>(
    "Tip",
    "The tip percentage",
    { "Calculator" },
    [this](ua::output& output)
    {
      output << tip;
    },
    [this](const ua::input& input)
    {
      input >> tip;

      std::cout << "New tip percentage is "
                << std::quoted(std::to_string(tip))
                << std::endl;
    });

  add_method(
    "Calculate",
    "Calculate the bill tip",
    { "Calculator" },
    {},
    { ua::argument::scalar<double>("Result", "The tip amount") },
    [this](const ua::input& input, ua::output& output)
    {
      // output << calculate();
      output << 42.0;
    });
}
