#include <reopen62541-cli/Argument.h>

#include <reopen62541-cli/Convert.h>

UA::Argument::Argument(String^ name, String^ description, uint32_t datatype) :
  argument_name(name),
  argument_description(description),
  argument_datatype(datatype),
  argument_dimensions(gcnew array<uint32_t>(0))
{
}

UA::Argument::Argument(String^ name, String^ description, uint32_t datatype, array<uint32_t>^ dimensions) :
  argument_name(name),
  argument_description(description),
  argument_datatype(datatype),
  argument_dimensions(dimensions)
{
}

String^ UA::Argument::Name::get()
{
  return argument_name;
}

String^ UA::Argument::Description::get()
{
  return argument_description;
}

generic<class T>
UA::Argument^ UA::Argument::Scalar(String^ name, String^ description)
{
  return gcnew UA::Argument(name, description, UA::Convert::ToUaDataType<T>());
}

generic<class T>
UA::Argument^ UA::Argument::Vector(String^ name, String^ description, const int dimension)
{
  return gcnew UA::Argument(name, description, UA::Convert::ToUaDataType<T>(), UA::Convert::ToArray<uint32_t>(dimension));
}

generic<class T>
UA::Argument^ UA::Argument::Array(String^ name, String^ description, array<int>^ dimensions)
{
  return gcnew UA::Argument(name, description, UA::Convert::ToUaDataType<T>(), UA::Convert::ToArray<uint32_t>(dimensions));
}

ua::argument UA::Argument::ToUaArgument()
{
  const auto name = UA::Convert::ToStdString(argument_name);
  const auto description = UA::Convert::ToStdString(argument_description);
  const auto datatype = argument_datatype;

  std::vector<uint32_t> dimensions(argument_dimensions->LongLength);

  for (long i = 0; i < argument_dimensions->LongLength; ++i)
  {
    dimensions[i] = argument_dimensions[i];
  }

  return ua::argument(name, description, datatype, dimensions);
}
