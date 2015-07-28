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

    else                                        loadPage("home");



    //Just for fun and to make the site look a bit cool ;)
    $("body > #topnav > div#menu-toggle")
      .css("background-size", "48px 48px")
      .css("background-position", "0px 0px");

    setTimeout(function()
    {
      $("body > #topnav > div#menu-toggle")
        .css("background-size", "32px 32px")
        .css("background-position", "8px 8px");
    }, 125);
    
    setTimeout(function()
    {
      $("body > #topnav > div#menu-toggle")
        .css("background-size", "48px 48px")
        .css("background-position", "0px 0px");
    }, 250);

    setTimeout(function()
    {
      $("body > #topnav > div#menu-toggle")
        .css("background-size", "32px 32px")
        .css("background-position", "8px 8px");
    }, 375);
    
    setTimeout(function()
    {
      $("body > #topnav > div#menu-toggle")
        .css("background-size", "48px 48px")
        .css("background-position", "0px 0px");
    }, 500);
    
    setTimeout(function()
    {
      $("body > #topnav > div#menu-toggle")
        .css("background-size", "32px 32px")
        .css("background-position", "8px 8px");
    }, 675);
  });
})();
