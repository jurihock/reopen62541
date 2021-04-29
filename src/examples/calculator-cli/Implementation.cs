using System;

class Calculator : ICalculator
{
  public double Bill { get; set; } = 0;
  public double Tip { get; set; } = 0;

  public double Calculate()
  {
    return Bill * Tip / 100.0;
  }
}
