#pragma once

#include <reopen62541/convert.h>
#include <reopen62541/strings.h>

#include <open62541.h>

#include <string>
#include <vector>

namespace ua
{
  class argument
  {
  public:

    argument(const std::string& name, const std::string& description, const uint32_t datatype) :
      argument_name(name),
      argument_description(description),
      argument_datatype(datatype),
      argument_dimensions({})
    {
    }

    argument(const std::string& name, const std::string& description, const uint32_t datatype, const std::vector<uint32_t>& dimensions) :
      argument_name(name),
      argument_description(description),
      argument_datatype(datatype),
      argument_dimensions(dimensions)
    {
    }

    argument(const ua::argument& other) :
      argument_name(other.argument_name),
      argument_description(other.argument_description),
      argument_datatype(other.argument_datatype),
      argument_dimensions(other.argument_dimensions)
    {
    }

    const std::string& name() const
    {
      return argument_name;
    }

    const std::string& description() const
    {
      return argument_description;
    }

    uint32_t datatype() const
    {
      return argument_datatype;
    }

    const std::vector<uint32_t>& dimensions() const
    {
      return argument_dimensions;
    }

    void to_ua_argument(char* const LC, ua::strings& STRINGS, UA_Argument& argument) const
    {
      // SCALAR

      if (argument_dimensions.empty())
      {
        UA_Argument_init(&argument);

        argument.name = UA_STRING(STRINGS(argument_name));
        argument.description = UA_LOCALIZEDTEXT(LC, STRINGS(argument_description));

        argument.dataType = UA_TYPES[argument_datatype].typeId;
        argument.valueRank = UA_VALUERANK_SCALAR;
      }

      // VECTOR

      else
      {
        UA_Argument_init(&argument);

        argument.name = UA_STRING(STRINGS(argument_name));
        argument.description = UA_LOCALIZEDTEXT(LC, STRINGS(argument_description));

        argument.dataType = UA_TYPES[argument_datatype].typeId;
        argument.valueRank = UA_VALUERANK_ONE_DIMENSION;

        argument.arrayDimensionsSize = argument_dimensions.size();
        argument.arrayDimensions = (uint32_t*)argument_dimensions.data();
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
    static ua::argument array(const std::string& name, const std::string& description, const std::vector<uint32_t>& dimensions)
    {
      return ua::argument(name, description, ua::convert::to_ua_data_type<T>(), dimensions);
    }

  private:

    const std::string argument_name;
    const std::string argument_description;
    const uint32_t argument_datatype;
    const std::vector<uint32_t> argument_dimensions;
  };
}
