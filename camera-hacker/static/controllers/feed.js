app.controller('feed', ['$scope', '$http', '$interval', function($scope, $http, $interval) {
    $scope.loaded = true;
    var cameraStates = {
        all: true
    };

    function reloadImage(imageId) {
        var img = document.getElementById(imageId);

        // True when image has loaded
        if (img.complete) {
            var src = img.src;

            // Remove existing query arguments from source
            var queryPos = src.indexOf('?');
            if (queryPos >= 0) {
                src = src.substr(0, queryPos);
            }

            // Set new query argument to current time
            // this doesn't do anything, other than convince the browser to load it again.
            img.src = src + '?time=' + new Date().getTime();
        }
    }

    $scope.loadImage = function loadImage(imageID) {
        if (cameraStates.all) {
            setTimeout(function () {reloadImage(imageID);}, 250);
        } else {
            $interval(function restartWhenStateChanges() {
                if (cameraStates.all) {
                    $scope.loadImage(imageID)
                }
            }, 5000);
        }
    };

    $interval(function checkGlobalState() {
        $http.get('/api/cameras').then(function (data) {
            cameraStates.all = data.data.cameraStates;
        });
    }, 5000);
}]);