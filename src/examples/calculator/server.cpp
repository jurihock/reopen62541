#include <calculator/server.h>

#include <chrono>
#include <condition_variable>
#include <csignal>
#include <mutex>
#include <thread>

std::mutex signal_mutex;
std::condition_variable signal_variable;

void signal_callback(int code)
{
  signal_variable.notify_all();
}

int main()
{
  std::unique_lock<std::mutex> signal_lock(signal_mutex);

  std::signal(SIGINT, signal_callback);
  std::signal(SIGTERM, signal_callback);

  calculator_server server;

  server.run_async();

  signal_variable.wait(signal_lock);

  server.shutdown();

  return 0;
}
