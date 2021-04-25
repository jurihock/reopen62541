#pragma once

#include <sstream>
#include <string>
#include <vector>

namespace ua
{
  class uid
  {
  public:

    const char delimiter;

    uid(char delimiter = '.') : delimiter(delimiter)
    {
    }

    const std::string to_string(const std::vector<std::string>& node_path) const
    {
      std::stringstream buffer;

      for (const auto& value : node_path)
        buffer << value << delimiter;

      std::string result = buffer.str();

      if (!result.empty())
        result.pop_back();

      return result;
    }

    const std::string to_string(const std::vector<std::string>& parent_node_path, const std::string& node_name) const
    {
      std::stringstream buffer;

      for (const auto& value : parent_node_path)
        buffer << value << delimiter;

      buffer << node_name;

      return buffer.str();
    }

    const std::string operator()(const std::vector<std::string>& node_path) const
    {
      return to_string(node_path);
    }

    const std::string operator()(const std::vector<std::string>& parent_node_path, const std::string& node_name) const
    {
      return to_string(parent_node_path, node_name);
    }
  };
}
