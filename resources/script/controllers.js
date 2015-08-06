;(function()
{
  //This controller constrols the entire body.
  angular.module("app").controller("main_controller", function($scope)
  {
  });
  
  //This controller controls the Authentication menu at the top of the screen
  angular.module("app").controller("topnav_authentication_menu", function($scope)
  {
    $scope.firstName = "Yash";
  });

  //This controller controls the sidebar, and what elements it contains
  angular.module("app").controller("sidebar", function($scope)
  {
    $scope.items =
    [
      {
        text: "Home"
      },
      {
        text: "Settings"
      }
    ];
  });

  ////This configuration enables routing in AngularJS
  //angular.module("app").config(function($routeProvider, $locationProvider)
  //{
    //$routeProvider
      //.when("/",
      //{
        //templateUrl: "pages/home.html",
        //resolve:
        //{
        //}
        //controller: "homeController" 
      //})
      //.when("/helloWorld",
      //{
        //templateUrl: "pages/helloworld.html"
      //});

    //$locationProvider.html5Mode(true);
  //});

  ////angular.module("app").controller("homeController", function($scope)
  ////{
    ////$scope.home = "Hi!";
  ////});
})();
