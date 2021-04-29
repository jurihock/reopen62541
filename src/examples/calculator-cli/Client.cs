using System;

static class Program
{
  static void Main()
  {
    System.Threading.Thread.Sleep(
      TimeSpan.FromSeconds(1));

    var client = new CalculatorClient();

    client.LogChanged += (sender, args) => Console.WriteLine(
      $"CLIENT [{args.Level} {args.Category}] {args.Message}");

    client.Connect();

    client.Bill = 100;
    client.Tip = 1;

    var bill = client.Bill;
    var tip = client.Tip;
    var obolus = client.Calculate();
    var total = bill + obolus;

    Console.WriteLine($"Bill amount \t {bill}");
    Console.WriteLine($"Tip percentage \t {tip}");
    Console.WriteLine($"Total amount \t {bill} + {tip} = {total}");

    client.Disconnect();
  }
}

class CalculatorClient : UA.Client, ICalculator
{
  public double Bill
  {
    get => Get<double>("Bill", new[] { "Calculator" });
    set => Set<double>("Bill", new[] { "Calculator" }, value);
  }

  public double Tip
  {
    get => Get<double>("Tip", new[] { "Calculator" });
    set => Set<double>("Tip", new[] { "Calculator" }, value);
  }

  public CalculatorClient() : base(4840, "localhost")
  {
  }

  public double Calculate()
  {
    double value = 0;

    Call("Calculate", new[] { "Calculator" },
      input => { },
      output => { value = output.Get<double>(); });

    return value;
  }
}
