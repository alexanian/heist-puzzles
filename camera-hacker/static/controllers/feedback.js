app.controller('login', ['$scope', '$http', '$window', function($scope, $http, $window) {
    $scope.formData = {
        feedback: null
    };

    $scope.error = null;
    $scope.message = null;

    $scope.feedback = function feedback() {
        $scope.error = null;
        $http.post('/api/feedback', $scope.formData).then(function (data) {
            $scope.message = data.data.message;
        }).catch(function(err) {
            $scope.error = err;
        });
    };
}]);