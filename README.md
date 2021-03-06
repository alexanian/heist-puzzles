# Heist Puzzles

Code for several electronic puzzles created for an interactive heist game.

## The Vault

This presently assumes you have a Particle Photon device connected up as in the [Motion Detector
Code Example](https://docs.particle.io/guide/getting-started/examples/photon/#make-a-motion-detector-publish-and-the-console).

You will need to enter the access token of your Particle device in a file `./the-vault/security-frontend/secrets.js`:
```
var accessToken = 'YOUR_ACCESS_TOKEN';
var deviceId = 'YOUR_DEVICE_ID';
```

The HTML page provides a security interface that responds to "alert" events in the browser.


## Camera Hacker

A puzzle for those who have always wanted to "hack the mainframe" to disable a "security system".

### Installing
* Install Python 3.5
* Create a virtual environment in the project folder with `virtualenv -p python3 venv`
* Activate the virtual environment with `. venv/bin/activate`
* Install packages with `pip install -r requirements.txt`

### Running the server
* For develop, `python server.py` (with venv enabled) will do ya
* To see it beyond your local machine, you're going to want to change the IP in Config.Server to
  0.0.0.0
* You will need to change the camera IPs in `Config/Cameras.py`
* I think debug mode messes up the challenge somehow, so I also recommend turning debug=False
  for actually running this. You can do that in `server.py`

### Setting up smartphone cameras

The system is currently configured for a single camera, though we'll want three in the final game.

You could purchase an IP webcam, but those are expensive, so instead we're following the advice of 
the MakeUseOf article [How to Build a Security Camera Network Out Of Old
Smartphones](https://www.makeuseof.com/tag/how-to-build-a-security-camera-network-out-of-old-smartphones/).
We installed BL-IP on an old smartphone and it began streaming video to an IP address configured
in the app, henceforth referred to as `YOUR_CAMERA_IP`.

#### Starting the image gathering script
Run (with venv activated) `python get_images.py` from the camera hacker directory. It needs valid
IPs in the config, then will take care of saving them to the disk. The API will then retrieve them.
This fixes a bug where two simaltaneous windows would break things. 

#### Checking that the camera is streaming

The BL-IP app makes available its current frame at `<YOUR_CAMERA_IP>/camera.jpg`. Test audio and
video streams can be viewed at `http://<YOUR_CAMERA_IP>/jpeg-browser.html`.

### Accessing the server
You can see the login page at `localhost:8080/login`. To access the feeds, enter the username and
password found in the Security configuration. If you go to /feeds without this, you'll be kicked
back to login. 
