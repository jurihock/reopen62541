using Microsoft.VisualStudio.TestTools.UnitTesting;
using System;

[TestClass]
public class ConnectionTest
{
  [TestMethod]
  public void NoConnectionTest()
  {
    using (var server = new UA.Server())
    using (var client = new UA.Client())
    {
      Assert.IsFalse(server.IsRunning);
      Assert.IsFalse(client.IsConnected);
    }
  }

  [TestMethod]
  public void GoodConnectionTest()
  {
    using (var server = new UA.Server())
    using (var client = new UA.Client())
    {
      for (var i = 0; i < 3; i++)
      {
        server.RunAsync();
        Assert.IsTrue(server.IsRunning);

        for (var j = 0; j < 3; j++)
        {
          client.Connect();
          Assert.IsTrue(client.IsConnected);

          client.Disconnect();
          Assert.IsFalse(client.IsConnected);
        }

        server.Shutdown();
        Assert.IsFalse(server.IsRunning);
      }
    }
  }

  [TestMethod]
  public void BadConnectionTest()
  {
    using (var client = new UA.Client())
    {
      try
      {
        client.Connect();
      }
      catch (UA.ClientException exception)
      {
        Assert.AreEqual(0x800A0000, exception.StatusCode);
        Assert.AreEqual("BadTimeout", exception.Message);
      }
      catch
      {
        Assert.Fail();
      }
      finally
      {
        Assert.IsFalse(client.IsConnected);
      }

      client.Disconnect();
      Assert.IsFalse(client.IsConnected);
    }
  }

  [TestMethod]
  public void DisposeConnectionTest()
  {
    for (var i = 0; i < 3; i++)
    {
      using (var server = new UA.Server())
      {
        server.RunAsync();
        Assert.IsTrue(server.IsRunning);

        using (var client = new UA.Client())
        {
          client.Connect();
          Assert.IsTrue(client.IsConnected);

          // don't disconnect client
        }

        // don't shutdown server
      }
    }
  }
}
