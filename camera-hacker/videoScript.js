// Script to show video based on BL IP-Camera
var cameraImageId = 'cameraImage';
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
        img.src = src + '?time=' + getNextTime();
    }
}

function getNextTime() {
    // Hackers could ""inject"" something else here
    // e.g. time - 5 minutes
    var date = new Date();
    return date.getTime();
}

function loadImage() {
    setTimeout(function () { reloadImage(cameraImageId); }, timeout);
}