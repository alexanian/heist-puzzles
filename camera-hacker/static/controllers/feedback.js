app.controller('login', ['$scope', '$http', '$sce', function($scope, $http, $sce) {
    $scope.formData = {
        feedback: null
    };

    $scope.error = null;
    $scope.message = null;

    $scope.feedback = function feedback() {
        $scope.error = null;
        $http.post('/api/feedback', $scope.formData).then(function (data) {
            $scope.message = data.data.message;
            $scope.message = $sce.trustAsHtml($scope.message);
        }).catch(function(err) {
            $scope.error = err;
        });
    };
}]);