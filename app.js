(function()
{
  //Import express JS and set up our app
  var express = require("express");
  var app = express();

  //Enable the server and begin listening on port 1234
  var server = app.listen(1234, function()
  {
    var host = server.address().address;
    var port = server.address().port;
  });

  //We want to have the resources directory be statically available
  app.use(express.static("resources"));

  //Return the index page as the fallback.
  app.get("*", function(req, res)
  {
    //read the index file and send it!
    res.sendFile(__dirname + "/resources/index.html");
  });
})();
