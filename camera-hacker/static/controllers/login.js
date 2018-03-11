app.controller('login', ['$scope', '$http', '$window', function($scope, $http, $window) {
    $scope.formData = {
        user: null,
        password: null
    };

    $scope.error = null;

    $scope.login = function login() {
        $scope.error = null;
        $http.post('/api/login', $scope.formData).then(function (data) {
            console.log(data);
            $window.location.href = '/feed?token=' + data.data.token;
        }).catch(function() {
            $scope.error = 'Incorrect username or password';
        });
    };
}]);