#include <calculator/client.h>

#include <chrono>
#include <condition_variable>
#include <csignal>
#include <mutex>
#include <thread>

int main()
{
  std::this_thread::sleep_for(
    std::chrono::seconds(1));

  calculator_client client;

  client.connect();

  std::cout << "Calculation result " << client.calculate();

  client.disconnect(); 
}
