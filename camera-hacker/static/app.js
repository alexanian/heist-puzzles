var app = angular.module('app', []);

app.config(function($interpolateProvider) {
    $interpolateProvider.startSymbol('{!');
    $interpolateProvider.endSymbol('!}');
});

// From https://stackoverflow.com/questions/17547917/angularjs-image-onload-event
app.directive('sbLoad', ['$parse', function ($parse) {
    return {
        restrict: 'A',
        link: function (scope, elem, attrs) {
            var fn = $parse(attrs.sbLoad);
            elem.on('load', function (event) {
                scope.$apply(function() {
                    fn(scope, { $event: event });
                });
            });
        }
    };
}]);