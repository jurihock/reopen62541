#pragma once

#include <cassert>
#include <map>
#include <mutex>
#include <string>
#include <vector>

namespace ua
{
  class strings
  {
  public:

    char* const to_ua_string(const std::string& value)
    {
      if (value.empty())
      {
        return empty;
      }

      std::lock_guard<std::mutex> lock(mutex);

      const auto existing = map.find(value);

      if (existing != map.end())
      {
        return existing->second.data();
      }

      std::vector<char> copy(value.size() + 1, 0); // incl. \0
      value.copy(copy.data(), value.size());
      const auto [created, ok] = map.emplace(value, copy);

      assert(ok);

      return created->second.data();
    }

    char* const operator()(const std::string& value)
    {
      return to_ua_string(value);
    }

  private:

    static inline char empty[] = "";

    std::map<std::string, std::vector<char>> map;
    std::mutex mutex;

  };
}
