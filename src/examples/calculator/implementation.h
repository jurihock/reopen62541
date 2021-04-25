#pragma once

#include <calculator/interface.h>

struct calculator_implementation : calculator_interface
{
  calculator_implementation() :
    calculator_bill(0),
    calculator_tip(0)
  {
  }

  double bill() override
  {
    return calculator_bill;
  }

  void bill(double bill) override
  {
    calculator_bill = bill;
  }

  double tip() override
  {
    return calculator_tip;
  }

  void tip(double tip) override
  {
    calculator_tip = tip;
  }

  double calculate() override
  {
    return calculator_bill * calculator_tip / 100.0;
  }

private:

  double calculator_bill;
  double calculator_tip;
};
