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