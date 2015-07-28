;(function()
{
  angular.module("app", ["ngRoute"]);

  $(document).ready(function()
  {
    var getScript = function(url)
    {
      jQuery.getScript(url);
    };
    var loadPage = function(name)
    {
      $.get("/pages/" + name + ".html", function(data)
      {
        $("body > #content").html(data);
        getScript("/pages/" + name + ".js");
      });
    };

    getScript("/script/drawer.js");
    getScript("/script/auth.js");

    var path = window.location.pathname;

//  Selector                                    Name
    if(path == "/" || path == "/home")          loadPage("home");
    if(path == "/settings")                     loadPage("settings");
  });
})();
