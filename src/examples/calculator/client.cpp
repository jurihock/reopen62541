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

  client.bill(100);
  client.tip(1);

  const auto bill = client.bill();
  const auto tip = client.tip();
  const auto obolus = client.calculate();
  const auto total = bill + obolus;

  std::cout << "Bill amount \t" << bill << std::endl;
  std::cout << "Tip percentage \t" << tip << "%" << std::endl;
  std::cout << "Total amount \t" << bill << " + " << obolus << " = " << total << std::endl;

  client.disconnect(); 
}
