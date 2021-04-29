using Microsoft.VisualStudio.TestTools.UnitTesting;
using System;

[TestClass]
public class BasicTest
{
  [TestMethod]
  public void ConnectionTest()
  {
    using (var server = new UA.Server())
    using (var client = new UA.Client())
    {
      Assert.IsFalse(server.IsRunning);
      Assert.IsFalse(client.IsConnected);

      server.RunAsync();
      Assert.IsTrue(server.IsRunning);
      
      client.Connect();
      Assert.IsTrue(client.IsConnected);

      client.Disconnect();
      Assert.IsFalse(client.IsConnected);

      server.Shutdown();
      Assert.IsFalse(server.IsRunning);
    }
  }
}
