using System;

interface ICalculator
{
  double Bill { get; set; }
  double Tip { get; set; }

  double Calculate();
};
