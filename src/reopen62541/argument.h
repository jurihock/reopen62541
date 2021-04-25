#pragma once

#include <reopen62541/convert.h>
#include <reopen62541/strings.h>

#include <stdexcept>
#include <string>
#include <vector>

#include <open62541.h>

namespace ua
{
  struct argument
  {
    const std::string name;
    const std::string description;
    const uint32_t datatype;
    const std::vector<uint32_t> dimensions;

    argument(const std::string& name, const std::string& description, const uint32_t datatype) :
      name(name), description(description), datatype(datatype), dimensions({})
    {
    }

    argument(const std::string& name, const std::string& description, const uint32_t datatype, const std::vector<uint32_t>& dimensions) :
      name(name), description(description), datatype(datatype), dimensions(dimensions)
    {
    }

    argument(const ua::argument& other) :
      name(other.name), description(other.description), datatype(other.datatype), dimensions(other.dimensions)
    {
    }

    void ToUaArgument(char* const LC, ua::strings& STRINGS, UA_Argument& argument) const
    {
      // SCALAR

      if (dimensions.empty())
      {
        UA_Argument_init(&argument);

        argument.name = UA_STRING(STRINGS(name));
        argument.description = UA_LOCALIZEDTEXT(LC, STRINGS(description));

        argument.dataType = UA_TYPES[datatype].typeId;
        argument.valueRank = UA_VALUERANK_SCALAR;
      }

      // VECTOR

      else
      {
        UA_Argument_init(&argument);

        argument.name = UA_STRING(STRINGS(name));
        argument.description = UA_LOCALIZEDTEXT(LC, STRINGS(description));

        argument.dataType = UA_TYPES[datatype].typeId;
        argument.valueRank = UA_VALUERANK_ONE_DIMENSION;

        argument.arrayDimensionsSize = dimensions.size();
        argument.arrayDimensions = (uint32_t*)dimensions.data();
      }
    }

    template<typename T>
    static ua::argument scalar(const std::string& name, const std::string& description)
    {
      return ua::argument(name, description, ua::convert::to_ua_data_type<T>());
    }

    template<typename T>
    static ua::argument vector(const std::string& name, const std::string& description, const uint32_t dimension)
    {
      return ua::argument(name, description, ua::convert::to_ua_data_type<T>(), { dimension });
    }

    template<typename T>
    static ua::argument ndarray(const std::string& name, const std::string& description, const std::vector<uint32_t>& dimensions)
    {
      return ua::argument(name, description, ua::convert::to_ua_data_type<T>(), dimensions);
    }
  };
}
