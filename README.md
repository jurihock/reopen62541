# reopen62541

reopen62541 is an attempt to extend the great [open62541](https://github.com/open62541/open62541) by the C++17 and C++/CLI interfaces, to hopefully minimize the syntactic overhead as well as provide a certain syntactic uniformity in C++ and C# environments. The goal is however not to support the complete feature set of the open62541 implementation or even all specified OPC UA nuances. In other words, reopen62541 is not designed for complexity, but for simplicity.

## Features

- Basic server and client instance management
- Callback based log message forwarding
- Callback based variable and method binding
- Template/generics based data type conversion
- Support of main data types like boolean, integers, floats and strings
- Support of scalars and vectors
- Unicode string handling

## Examples

### Create server and client instances

<table>
<tr>
<th>C++</th>
<th>C#</th>
</tr>
<tr>
<td>
<pre>
ua::server server;<br>
server.run_async();
server.shutdown();<br>
ua::client client;<br>
client.connect();
client.disconnect();
</pre>
</td>
<td>
<pre>
var server = new UA.Server();<br>
server.RunAsync();
server.Shutdown();<br>
var client = new UA.Client();<br>
client.Connect();
client.Disconnect();
</pre>
</td>
</tr>
</table>

### Create objects and folders

<table>
<tr>
<th>C++</th>
<th>C#</th>
</tr>
<tr>
<td>
<pre>
server.add_object(
  "MyObject",
  "The root object");<br>
server.add_folder(
  "MyFolder",
  "Folder inside object",
  { "MyObject" });
</pre>
</td>
<td>
<pre>
server.AddObject(
  "MyObject",
  "The root object");<br>
server.AddFolder(
  "MyFolder",
  "Folder inside object",
  new[] { "MyObject" });
</pre>
</td>
</tr>
</table>

### Implement variables on the server side

<table>
<tr>
<th>C++</th>
<th>C#</th>
</tr>
<tr>
<td>
<pre>
int variable = 0;<br>
server.add_variable&lt;int&gt;(
  "MyVariable",
  "Variable inside folder",
  { "MyObject", "MyFolder" },
  [&](ua::variant& output)
  {
    output.set&lt;int&gt;(variable);
  },
  [&](const ua::variant& input)
  {
    variable = input.get&lt;int&gt;();
  });
</pre>
</td>
<td>
<pre>
int variable = 0;<br>
server.AddVariable&lt;int&gt;(
  "MyVariable",
  "Variable inside folder",
  new[] { "MyObject", "MyFolder" },
  output =>
  {
    output.Set&lt;int&gt;(variable);
  },
  input =>
  {
    variable = input.Get&lt;int&gt;();
  });
</pre>
</td>
</tr>
</table>

### Access variables on the client side

<table>
<tr>
<th>C++</th>
<th>C#</th>
</tr>
<tr>
<td>
<pre>
int variable = 0;<br>
variable = client.get&lt;int&gt;(
  "MyVariable",
  { "MyObject", "MyFolder" });<br>
client.set&lt;int&gt;(
  "MyVariable",
  { "MyObject", "MyFolder" },
  variable);
</pre>
</td>
<td>
<pre>
int variable = 0;<br>
variable = client.Get&lt;int&gt;(
  "MyVariable",
  new[] { "MyObject", "MyFolder" }); <br>
client.Set&lt;int&gt;(
  "MyVariable",
  new[] { "MyObject", "MyFolder" },
  variable);
</pre>
</td>
</tr>
</table>

# License

This Source Code Form is subject to the terms of the Mozilla Public License 2.0. The file [LICENSE](LICENSE) contains a copy of the MPL. Alternatively obtain another one at [https://mozilla.org/MPL/2.0](https://mozilla.org/MPL/2.0).
