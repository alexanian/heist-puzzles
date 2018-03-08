# Heist Puzzles

Code for several electronic puzzles created for an interactive heist game.

## Laser Maze

This presently assumes you have a Particle Photon device connected up as in the [Motion Detector
Code Example](https://docs.particle.io/guide/getting-started/examples/photon/#make-a-motion-detector-publish-and-the-console).

You will need to enter the access token of your Particle device in a file `./laser-maze/secrets.js`:
```
var accessToken = 'YOUR_ACCESS_TOKEN';
```

The `testResistorEvents` HTML page allows you to see the results of the `beamStatus` event in the browser.


## Camera Hacker

A puzzle for those who have always wanted to "hack the mainframe" to disable a "security system".

### Setting up smartphone cameras

The system is currently configured for a single camera, though we'll want three in the final game.

You could purchase an IP webcam, but those are expensive, so instead we're following the advice of 
the MakeUseOf article [How to Build a Security Camera Network Out Of Old
Smartphones](https://www.makeuseof.com/tag/how-to-build-a-security-camera-network-out-of-old-smartphones/).
We installed BL-IP on an old smartphone and it began streaming video to an IP address configured
in the app, henceforth referred to as `YOUR_CAMERA_IP`.

#### Checking that the camera is streaming

The BL-IP app makes available its current frame at `<YOUR_CAMERA_IP>/camera.jpg`. Test audio and
video streams can be viewed at `http://<YOUR_CAMERA_IP>/jpeg-browser.html`.