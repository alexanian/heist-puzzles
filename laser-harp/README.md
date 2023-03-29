# Laser Harp

Arduino-compatible microcontroller code for a laser harp, built for Heist 2023: Sunlit Forge. The microcontroller reads values from 5 photoresistors; each photoresistor is aligned to a laser that functions as a harp string. When the string is "plucked", the laser's beam is interrupted, resulting in a lower voltage across the photoresistor. The microcontroller reads this and plays a corresponding MIDI note.

When a specific six-note sequence is played, the microcontroller activates a solenoid lock, unlocking a trapdoor.

~~~~## Design & Components

The laser harp in 2023 was constructed using the following components:

* [Jolooyo LAB 532-10 5VDC Green Laser](https://www.amazon.com/dp/B07D9BTRCV)
* [Adafruit Metro 328](https://www.adafruit.com/product/2488), an Arduino Uno clone
* [Adafruit "Music Maker" MP3 Shield](https://www.adafruit.com/product/1788)

For the solenoid trapdoor lock:

* [Sainsmart 5V 2-Channel Relay Module](https://www.sainsmart.com/products/2-channel-5v-relay-module)
* [Adafruit Lock-Style Solenoid 12V DC](https://www.adafruit.com/product/1512)

For a simplified circuits diagram, see [this photo](https://photos.app.goo.gl/uYjtmbHospFB3xFBA).

## Development & Pinout Setup

This code uses the Adafruit VS1053 library to synthesize MIDI. Install it [here](https://learn.adafruit.com/adafruit-music-maker-shield-vs1053-mp3-wav-wave-ogg-vorbis-player/installing-software).

This code was tested on an Adafruit Metro 328, which is an Arduino Uno clone. It should probably work on an Arduino Uno too, but this hasn't been tested. The expected pin setup is as follows:

- Analog I/O 1-5: Photoresistors
- Digital I/O 2: Adafruit Music Maker MP3 Shield
- Digital I/O 3: Lasers (PWM)
- Digital I/O 7: Relay module that controls solenoid lock
  
The Adafruit Music Maker MP3 Shield requires a few jumper connections to function as a MIDI synth; as described [here](https://learn.adafruit.com/adafruit-music-maker-shield-vs1053-mp3-wav-wave-ogg-vorbis-player/midi-synth): 

- Solder 3V to GPIO1
- Solder Digital I/O 2 to RX

