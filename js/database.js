(function()
{
  var mysql = require(config.app.module_root_path + "mysql");
  var connection = null;
  var dbconf = null;
  var fs = require("fs");
  module.exports.init = function()
  {
    dbconf = JSON.parse(fs.readFileSync("configuration/database.json", "utf-8"));
    connection = mysql.createConnection(
    {
      host: dbconf.host,
      user: dbconf.user,
      password: dbconf.password,
      database: dbconf.database
    });

    connection.connect();
  };

  module.exports.query = function(query, callback)
  {
    connection.query(query, function(err, rows, fields)
    {
      callback(err, rows, fields);
    });
  };
})();
