(function()
{
  angular.module("app").controller("topnav_authentication_menu", function($scope)
  {
    $scope.firstName = "Yash";
  });

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

  angular.module("app").controller("main_controller", function($scope)
  {
  });


  angular.module("app").config(function($routeProvider, $locationProvider)
  {
    $routeProvider
      .when("/",
      {
        templateUrl: "pages/home.html",
        controller: "homeController"
      })
      .when("/helloWorld",
      {
        templateUrl: "pages/helloworld.html"
      });

    $locationProvider.html5Mode(true);
  });

  angular.module("app").controller("homeController", function($scope)
  {
    $scope.home = "Hi!";
  });
})();
