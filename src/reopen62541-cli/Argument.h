#pragma once

#include <reopen62541/argument.h>

#using <System.dll>

using namespace System;

namespace UA
{
  public ref class Argument
  {
  internal:

    Argument(String^ name, String^ description, uint32_t datatype);
    Argument(String^ name, String^ description, uint32_t datatype, array<uint32_t>^ dimensions);

    ua::argument ToUaArgument();

  public:

    property String^ Name
    {
      String^ get();
    }

    property String^ Description
    {
      String^ get();
    }

    // TODO: data type & dimensions 

    generic<class T>
    static UA::Argument^ Scalar(String^ name, String^ description);

    generic<class T>
    static UA::Argument^ Vector(String^ name, String^ description, const int dimension);

    generic<class T>
    static UA::Argument^ Array(String^ name, String^ description, array<int>^ dimensions);

  private:

    String^ argument_name;
    String^ argument_description;
    uint32_t argument_datatype;
    array<uint32_t>^ argument_dimensions;
  };
}
