// Script to show video based on BL IP-Camera
var timeout = 50; // milliseconds

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
        img.src = src + '?time=' + getNextTime();
    }
}

function getNextTime() {
    return new Date().getTime();
}

function loadImage(imageID) {
    setTimeout(function () {reloadImage(imageID);}, timeout);
}