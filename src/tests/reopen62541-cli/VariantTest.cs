using Microsoft.VisualStudio.TestTools.UnitTesting;
using System;
using System.Linq;

[TestClass]
public class VariantTest
{
  static readonly string[] Scalars = { "Scalars" };

  [TestMethod]
  public void UnicodeTest()
  {
    using (var server = new UA.Server())
    {
      server.AddObject(Scalars.First(), Scalars.First());

      server.AddVariable<string>("en", "English", Scalars, () => "Hello World!");
      server.AddVariable<string>("de", "Deutsch", Scalars, () => "Hallö Wält!");
      server.AddVariable<string>("fr", "Français", Scalars, () => "Hællo Wœrld!");
      server.AddVariable<string>("ru", "Русский", Scalars, () => "Превед Медвед!");
      server.AddVariable<string>("ja", "日本語", Scalars, () => "アニメ映画!");

      server.RunAsync();

      using (var client = new UA.Client())
      {
        client.Connect();

        Assert.AreEqual("Hello World!", client.Get<string>("en", Scalars));
        Assert.AreEqual("Hallö Wält!", client.Get<string>("de", Scalars));
        Assert.AreEqual("Hællo Wœrld!", client.Get<string>("fr", Scalars));
        Assert.AreEqual("Превед Медвед!", client.Get<string>("ru", Scalars));
        Assert.AreEqual("アニメ映画!", client.Get<string>("ja", Scalars));
      }
    }
  }

  [TestMethod]
  public void ScalarTest()
  {
    using (var server = new UA.Server())
    {
      server.AddObject(Scalars.First(), Scalars.First());

      server.AddVariable<bool>(nameof(Boolean), nameof(Boolean), Scalars, () => true);

      server.AddVariable<sbyte>(nameof(SByte), nameof(SByte), Scalars, () => -1);
      server.AddVariable<byte>(nameof(Byte), nameof(Byte), Scalars, () => +1);

      server.AddVariable<short>(nameof(Int16), nameof(Int16), Scalars, () => -1);
      server.AddVariable<ushort>(nameof(UInt16), nameof(UInt16), Scalars, () => +1);

      server.AddVariable<int>(nameof(Int32), nameof(Int32), Scalars, () => -1);
      server.AddVariable<uint>(nameof(UInt32), nameof(UInt32), Scalars, () => +1U);

      server.AddVariable<long>(nameof(Int64), nameof(Int64), Scalars, () => -1L);
      server.AddVariable<ulong>(nameof(UInt64), nameof(UInt64), Scalars, () => +1UL);

      server.AddVariable<float>(nameof(Single), nameof(Single), Scalars, () => -1.0f);
      server.AddVariable<double>(nameof(Double), nameof(Double), Scalars, () => +1.0d);

      server.AddVariable<string>(nameof(String), nameof(String), Scalars, () => "string");

      server.RunAsync();

      using (var client = new UA.Client())
      {
        client.Connect();

        Assert.IsTrue(client.Get<bool>(nameof(Boolean), Scalars));

        Assert.AreEqual(-1, client.Get<sbyte>(nameof(SByte), Scalars));
        Assert.AreEqual(+1, client.Get<byte>(nameof(Byte), Scalars));

        Assert.AreEqual(-1, client.Get<short>(nameof(Int16), Scalars));
        Assert.AreEqual(+1, client.Get<ushort>(nameof(UInt16), Scalars));

        Assert.AreEqual(-1, client.Get<int>(nameof(Int32), Scalars));
        Assert.AreEqual(+1U, client.Get<uint>(nameof(UInt32), Scalars));

        Assert.AreEqual(-1L, client.Get<long>(nameof(Int64), Scalars));
        Assert.AreEqual(+1UL, client.Get<ulong>(nameof(UInt64), Scalars));

        Assert.AreEqual(-1.0f, client.Get<float>(nameof(Single), Scalars));
        Assert.AreEqual(+1.0d, client.Get<double>(nameof(Double), Scalars));

        Assert.AreEqual("string", client.Get<string>(nameof(String), Scalars));
      }
    }
  }
}
