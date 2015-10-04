;(function()
{
  angular.module("app", ["ngRoute"]);
  var lck_psel = false;

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

      lck_psel = true;
    };

    getScript("/script/drawer.js");
    getScript("/script/auth.js");

    var path = window.location.pathname;

    console.log(path);

//  Selector                                    Name
    if(path == "/" || path == "/home")          loadPage("home");
    if(path == "/settings")                     loadPage("settings");
    if(path == "/notes")                        loadPage("notes");

    if(!lck_psel)                               loadPage("home");



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
  });
})();
