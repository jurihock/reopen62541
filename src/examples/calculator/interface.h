#pragma once

struct calculator_interface
{
  ~calculator_interface() {}

  virtual double bill() = 0;
  virtual void bill(double) = 0;

  virtual double tip() = 0;
  virtual void tip(double) = 0;

  virtual double calculate() = 0;
};
