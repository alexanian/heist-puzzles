// Particle event detection, borrowed from this tutorial:
// https://community.particle.io/t/tutorial-getting-started-with-spark-publish/3422
// accessToken should be imported from another (secret) file
function start() {
    var eventSource = new EventSource("https://api.spark.io/v1/devices/events/?access_token=" + accessToken);

    eventSource.addEventListener('alert', function(e) {
        var parsedData = JSON.parse(e.data);
        var tempSpan = document.getElementById("status");
        tempSpan.innerHTML = "Status: " + parsedData.data;
        tempSpan.style.fontSize = "28px";
        
        var tsSpan = document.getElementById("ts");
        tsSpan.innerHTML = "At timestamp " + parsedData.published_at;
        tsSpan.style.fontSize = "9px";
    }, false);
}