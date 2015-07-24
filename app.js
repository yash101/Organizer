var express = require("express");
var app = express();

var server = app.listen(1234, function()
{
  var host = server.address().address;
  var port = server.address().port;

  console.log("Server stared listening at: http://%s:%s", host, port);
});

app.use(express.static('resources'));

app.get("*", function(req, res)
{
  res.sendFile(__dirname + "/resources/index.html");
});
