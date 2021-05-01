using Microsoft.VisualStudio.TestTools.UnitTesting;
using System;
using System.Linq;

[TestClass]
public class VariantTest
{
  static readonly string[] Scalars = { "Scalars" };
  static readonly string[] Vectors = { "Vectors" };

  [TestMethod]
  public void EmptyStringTest()
  {
    // empty scalar

    using (var server = new UA.Server())
    {
      server.AddObject(Scalars.First(), Scalars.First());

      server.AddVariable<string>(nameof(String), nameof(String), Scalars, () => "");

      server.RunAsync();

      using (var client = new UA.Client())
      {
        client.Connect();

        Assert.AreEqual("", client.Get<string>(nameof(String), Scalars));
      }
    }

    // vector with an empty scalar

    using (var server = new UA.Server())
    {
      server.AddObject(Vectors.First(), Vectors.First());

      server.AddVariable<string[]>(nameof(String), nameof(String), Vectors, () => new[] { "" });

      server.RunAsync();

      using (var client = new UA.Client())
      {
        client.Connect();

        Assert.AreEqual(1, client.Get<string[]>(nameof(String), Vectors).Length);
        Assert.AreEqual("", client.Get<string[]>(nameof(String), Vectors).First());
      }
    }

    // empty vector

    using (var server = new UA.Server())
    {
      server.AddObject(Vectors.First(), Vectors.First());

      server.AddVariable<string[]>(nameof(String), nameof(String), Vectors, () => new string[0]);

      server.RunAsync();

      using (var client = new UA.Client())
      {
        client.Connect();

        Assert.AreEqual(0, client.Get<string[]>(nameof(String), Vectors).Length);
      }
    }
  }

  [TestMethod]
  public void UnicodeStringTest()
  {
    var en = "Hello World!";
    var de = "Hallö Wält!";
    var fr = "Hællo Wœrld!";
    var ru = "Превед Медвед!";
    var ja = "アニメ映画!";

    using (var server = new UA.Server())
    {
      server.AddObject(Scalars.First(), Scalars.First());

      server.AddVariable<string>("en", "English", Scalars, () => en);
      server.AddVariable<string>("de", "Deutsch", Scalars, () => de);
      server.AddVariable<string>("fr", "Français", Scalars, () => fr);
      server.AddVariable<string>("ru", "Русский", Scalars, () => ru);
      server.AddVariable<string>("ja", "日本語", Scalars, () => ja);

      server.RunAsync();

      using (var client = new UA.Client())
      {
        client.Connect();

        Assert.AreEqual(en, client.Get<string>("en", Scalars));
        Assert.AreEqual(de, client.Get<string>("de", Scalars));
        Assert.AreEqual(fr, client.Get<string>("fr", Scalars));
        Assert.AreEqual(ru, client.Get<string>("ru", Scalars));
        Assert.AreEqual(ja, client.Get<string>("ja", Scalars));
      }
    }

    using (var server = new UA.Server())
    {
      var input = new[] { en, de, fr, ru, ja };

      server.AddObject(Vectors.First(), Vectors.First());

      server.AddVariable<string[]>("...", "...", Vectors, () => input);

      server.RunAsync();

      using (var client = new UA.Client())
      {
        client.Connect();

        var output = client.Get<string[]>("...", Vectors);

        Assert.AreEqual(input.Length, output.Length);

        for (var i = 0; i < input.Length; i++)
        {
          Assert.AreEqual(input[i], output[i]);
        }
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

  [TestMethod]
  public void VectorTest()
  {
    var BooleanValues = new bool[2] { true, false };

    var SByteValues = new sbyte[2] { sbyte.MinValue, sbyte.MaxValue };
    var ByteValues = new byte[2] { byte.MinValue, byte.MaxValue };

    var Int16Values = new short[2] { short.MinValue, short.MaxValue };
    var UInt16Values = new ushort[2] { ushort.MinValue, ushort.MaxValue };

    var Int32Values = new int[2] { int.MinValue, int.MaxValue };
    var UInt32Values = new uint[2] { uint.MinValue, uint.MaxValue };

    var Int64Values = new long[2] { long.MinValue, long.MaxValue };
    var UInt64Values = new ulong[2] { ulong.MinValue, ulong.MaxValue };

    var SingleValues = new float[2] { float.MinValue, float.MaxValue };
    var DoubleValues = new double[2] { double.MinValue, double.MaxValue };

    var StringValues = new string[2] { "foo", "bar" };

    using (var server = new UA.Server())
    {
      server.AddObject(Vectors.First(), Vectors.First());

      server.AddVariable<bool[]>(nameof(Boolean), nameof(Boolean), Vectors, () => BooleanValues);

      server.AddVariable<sbyte[]>(nameof(SByte), nameof(SByte), Vectors, () => SByteValues);
      server.AddVariable<byte[]>(nameof(Byte), nameof(Byte), Vectors, () => ByteValues);

      server.AddVariable<short[]>(nameof(Int16), nameof(Int16), Vectors, () => Int16Values);
      server.AddVariable<ushort[]>(nameof(UInt16), nameof(UInt16), Vectors, () => UInt16Values);

      server.AddVariable<int[]>(nameof(Int32), nameof(Int32), Vectors, () => Int32Values);
      server.AddVariable<uint[]>(nameof(UInt32), nameof(UInt32), Vectors, () => UInt32Values);

      server.AddVariable<long[]>(nameof(Int64), nameof(Int64), Vectors, () => Int64Values);
      server.AddVariable<ulong[]>(nameof(UInt64), nameof(UInt64), Vectors, () => UInt64Values);

      server.AddVariable<float[]>(nameof(Single), nameof(Single), Vectors, () => SingleValues);
      server.AddVariable<double[]>(nameof(Double), nameof(Double), Vectors, () => DoubleValues);

      server.AddVariable<string[]>(nameof(String), nameof(String), Vectors, () => StringValues);

      server.RunAsync();

      using (var client = new UA.Client())
      {
        client.Connect();

        for (var i = 0; i < 2; i++)
        {
          Assert.AreEqual(BooleanValues[i], client.Get<bool[]>(nameof(Boolean), Vectors)[i]);

          Assert.AreEqual(SByteValues[i], client.Get<sbyte[]>(nameof(SByte), Vectors)[i]);
          Assert.AreEqual(ByteValues[i], client.Get<byte[]>(nameof(Byte), Vectors)[i]);

          Assert.AreEqual(Int16Values[i], client.Get<short[]>(nameof(Int16), Vectors)[i]);
          Assert.AreEqual(UInt16Values[i], client.Get<ushort[]>(nameof(UInt16), Vectors)[i]);

          Assert.AreEqual(Int32Values[i], client.Get<int[]>(nameof(Int32), Vectors)[i]);
          Assert.AreEqual(UInt32Values[i], client.Get<uint[]>(nameof(UInt32), Vectors)[i]);

          Assert.AreEqual(Int64Values[i], client.Get<long[]>(nameof(Int64), Vectors)[i]);
          Assert.AreEqual(UInt64Values[i], client.Get<ulong[]>(nameof(UInt64), Vectors)[i]);

          Assert.AreEqual(SingleValues[i], client.Get<float[]>(nameof(Single), Vectors)[i]);
          Assert.AreEqual(DoubleValues[i], client.Get<double[]>(nameof(Double), Vectors)[i]);

          Assert.AreEqual(StringValues[i], client.Get<string[]>(nameof(String), Vectors)[i]);
        }
      }
    }
  }
}
