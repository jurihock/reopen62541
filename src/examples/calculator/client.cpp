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

