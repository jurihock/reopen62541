using System;

static class Program
{
  static void Main()
  {
    var server = new CalculatorServer();

    server.LogChanged += (sender, args) => Console.WriteLine(
      $"SERVER [{args.Level} {args.Category}] {args.Message}");

    server.RunAsync();

    Console.ReadKey(intercept: true);

    server.Shutdown();
  }
}

class CalculatorServer : UA.Server, ICalculator
{
  private readonly Calculator Calculator = new Calculator();

  public double Bill { get => Calculator.Bill; set => Calculator.Bill = value; }
  public double Tip { get => Calculator.Tip; set => Calculator.Tip = value; }

  public CalculatorServer() : base(4840, "localhost", "The Bill Tip Calculator", "urn:calculator")
  {
    AddObject(
      "Calculator",
      "The Bill Tip Calculator");

    AddVariable<double>(
      "Bill",
      "The bill amount",
      new[]{ "Calculator" },
      () => Bill,
      value =>
      {
        Bill = value;

        Console.WriteLine($"New bill amount is \"{Bill}\"");
      });

    AddVariable<double>(
      "Tip",
      "The tip percentage",
      new[] { "Calculator" },
      () => Tip,
      value =>
      {
        Tip = value;

        Console.WriteLine($"New tip percentage is \"{Tip}\"");
      });

    AddMethod(
      "Calculate",
      "Calculate the bill tip",
      new[] { "Calculator" },
      null,
      new[] { UA.Argument.Scalar<double>("Result", "The tip amount") },
      (input, output) =>
      {
        Console.WriteLine($"Calculating the bill tip...");

        output.Set<double>(Calculate());
      });
  }

  public double Calculate()
  {
    return Calculator.Calculate();
  }
}
