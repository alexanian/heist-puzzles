// Security system button and Particle device events
//
// Based particle on the Particle event detection tutorial:
// https://community.particle.io/t/tutorial-getting-started-with-spark-publish/3422
var secondsSinceAlert = 0;
var inAlertMode = true;

// accessToken should be includes in secrets.js, which is not versioned
var pressureTrapURL = "https://api.spark.io/v1/devices/" + pressureTrapDeviceId + "/";
var laserMazeURL = "https://api.spark.io/v1/devices/" + laserMazeDeviceId + "/";
var eventsURL = "https://api.spark.io/v1/devices/events/?access_token=" + accessToken;

// Begin monitoring the event sources and updating the timespace
$(document).ready( function() {

    // Set up events
    var eventSource = new EventSource(eventsURL);
    eventSource.addEventListener('alert-pressure-trap', function (e) {
        alert();
    }, false);

    eventSource.addEventListener('alert-laser-maze', function (e) {
       alert();
    }, false);

    // Update the seconds since alert
    setInterval(function() {
        secondsSinceAlert++;
        setTime();
    }, 1000);

    setActive();
    flashAlert(false);
});

function reset() {
    setActive();

    $.post( pressureTrapURL + "reset/?access_token=" + accessToken, function() {
        secondsSinceAlert = 0;
        setTime();
    });

    $.post (laserMazeURL + "reset/?access_token=" + accessToken, function() {
        // Only reset the page once both posts have succeeded
        var alertDiv = document.getElementById("alert-text");
        alertDiv.innerText = "No active alerts.";
        alertDiv.classList.remove("flashing");
        
        secondsSinceAlert = 0;
        setTime();
    });

    // Set up events
    var eventSource = new EventSource(eventsURL);
    eventSource.addEventListener('alert', function (e) {
        alert();
    }, false);    
}

function setTime() {
    // Update the "it has been <> hours since" 
    var timeSpan = document.getElementById("time-since-breach");
    timeSpan.innerHTML = new Date(secondsSinceAlert * 1000).toISOString().substr(11, 8);    
}

function setActive() {
    // Set "system is active" text
    var activeSpan = document.getElementById("is-active");
    activeSpan.innerHTML = "active";
}

function alert() {
    // No time has passed since the last alert!
    secondsSinceAlert = 0;
    setTime();

    // Yeah dude start warning everyone!
    var alertDiv = document.getElementById("alert-text");
    alertDiv.innerText = "ALERT! Breach detected!";
    flashAlert(true);
}

function flashAlert(addFlash) {
    var alertDiv = document.getElementById("alert-text");

    if (addFlash) {
        alertDiv.classList.add("flashing");
    }
    else {
        alertDiv.classList.remove("flashing");
    }
}
