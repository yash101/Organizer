;(function()
{
  angular.module("app", []);

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

//My homebrew (and sorta crappy) router :)
//  Selector                                    Name
    if(path == "/" || path == "/home")          loadPage("home");
    if(path == "/settings")                     loadPage("settings");
    if(path == "/notes")                        loadPage("notes");

    else                                        loadPage("home");
  });
})();
