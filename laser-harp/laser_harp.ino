  /*
  Switches a relay module (to unlock a solenoid lock) when the correct melody is played
  Uses the Adafruit VS1053 Codec library`
  */

  #define VS1053_RX  2 // This is the pin that connects to the RX pin on VS1053

  // See http://www.vlsi.fi/fileadmin/datasheets/vs1053.pdf Pg 31
  #define VS1053_BANK_MELODY 0x79

  // See http://www.vlsi.fi/fileadmin/datasheets/vs1053.pdf Pg 32 for more!
  #define VS1053_INSTRUMENT 1

  #define MIDI_NOTE_ON  0x90
  #define MIDI_NOTE_OFF 0x80
  #define MIDI_CHAN_MSG 0xB0
  #define MIDI_CHAN_BANK 0x00
  #define MIDI_CHAN_VOLUME 0x07
  #define MIDI_CHAN_PROGRAM 0xC0

  const int LASER_OUT_PIN = 3;
  const int LASER_ON_PWM_VALUE =  230; // 192 = 75% of 255, for a 75% duty cycle, 192
  const int LASER_OFF_PWM_VALUE = 0;
  const int RELAY_OUT_PIN = 7;
  const int inputPins[5] = {A0, A1, A2, A3, A4};
  const int notes[5] = {60, 62, 64, 67, 69}; //  Major pentatonic on middle C
  const int correctMelody[6] = {60, 62, 60, 67, 69, 67};

  int reading;
  int pinThresholds[5];  // Threshold to convert analog photoresistor readings to digital values
  int pinStates[5] = {0, 0, 0, 0, 0};
  int previousPinStates[5] = {0, 0, 0, 0, 0};
  int melody[6];
  int melodyIndex = 0;
  bool melodiesMatch = false;

  #if defined(__AVR_ATmega328__) || defined(__AVR_ATmega328P__)
    #include <SoftwareSerial.h>
    SoftwareSerial VS1053_MIDI(0, 2); // TX only, do not use the 'rx' side
  #else
    // on a Mega/Leonardo you may have to change the pin to one that 
    // software serial support uses OR use a hardware serial port!
    #define VS1053_MIDI Serial1
  #endif

  void setup() {
    Serial.begin(9600);
    VS1053_MIDI.begin(31250); // MIDI uses a 'strange baud rate'
    
    midiSetChannelBank(0, VS1053_BANK_MELODY);
    midiSetInstrument(0, VS1053_INSTRUMENT);
    midiSetChannelVolume(0, 127);

    // Set relay pin to out
    pinMode(RELAY_OUT_PIN, OUTPUT);
    digitalWrite(RELAY_OUT_PIN, HIGH);

    // Calibrate photoresistor values to determine thresholds for
    // lasers being on vs off
    calibrateInputPins();
  }

  void loop() {
    monitorMelody();
    delay(30); 

    // Relay test code
    //// unlock Solenoid = HIGH, lock = LOW
    // digitalWrite(RELAY_OUT_PIN, HIGH);
    // delay(1000);
    // digitalWrite(RELAY_OUT_PIN, LOW);
    // delay(5000);
  }

  void monitorMelody() {
    /*
     * Reads the state of all input pins. If a pin's reading is below its threshold, assumes
     * the laser is interrupted, and plays the corresponding note.
     * After each note, check whether the correct melody has been played.
     */
      for (byte i = 0; i < 5; i = i + 1) {
        // Read the input pin
        reading = analogRead(inputPins[i]);
        // Serial.print(i);
        // Serial.print(": ");
        // Serial.println(reading);
        pinStates[i] = reading > pinThresholds[i] ? 1 : 0; // TODO(Samira) set back to pinThresholds[i]

        // If the input pin state has changed, play (or silence)
        // the corresponding note
        if (pinStates[i] != previousPinStates[i]) {
          if (pinStates[i] == 1) {
            midiNoteOff(0, notes[i], 127);
          } else {
            midiNoteOn(0, notes[i], 127);

            // Add to the running memory of the melody
            melody[melodyIndex] = notes[i];
            
            // Increment melodyIndex, ring-buffer-style, so that
            // it always points to the oldest note in melody
            if (melodyIndex < 5) { 
              melodyIndex++;
            } else {
              melodyIndex = 0;
            }
            melodiesMatch = checkMelody();

            // Light up the LED if the correct melody is played
            if (melodiesMatch) {
              // Let the final note fade
              delay(500);
              midiNoteOff(0, notes[i], 127);

              // Play the melody once more as a success sound
              for (byte j = 0; j < 6; j = j + 1) {
                midiNoteOn(0, correctMelody[j], 127);
                if (j == 4 || j == 5) { delay(800); } else { delay(400); }
                midiNoteOff(0, correctMelody[j], 127);
              }
              
              // Unlock the solenoid
              digitalWrite(RELAY_OUT_PIN, LOW);
              delay(200);
              digitalWrite(RELAY_OUT_PIN, HIGH);
            }
          }
        }
      previousPinStates[i] = pinStates[i];
    }
  }

  void getCalibrationValues(int arr[]) {
    for (int i = 0; i < 5; i = i + 1) {
      int analogReading = analogRead(inputPins[i]);
      Serial.print("Pin ");
      Serial.print(i);
      Serial.print(": ");
      Serial.println(analogReading);
      arr[i] = analogReading;
    }
  }

  void calibrateInputPins() {
    /**
     * The pins are analog inputs, but we want to convert them to a digital signal (laser interrupted or not).
     * This method takes a reading for each pin with the lasers on, and one with the lasers interrupted (this 
     * must be done manually), and uses the midpoint of those values as a threshold for determining HIGH vs LOW.
     */
    Serial.println("Begin calibration. Setting lasers ON to read HIGH values of input pins...");
    analogWrite(LASER_OUT_PIN, LASER_ON_PWM_VALUE);
    delay(300);
    int inputHighValues[5];
    getCalibrationValues(inputHighValues);
    delay(100);

    Serial.println("Setting lasers OFF to read LOW values of input pins...");
    analogWrite(LASER_OUT_PIN, LASER_OFF_PWM_VALUE);
    delay(300);
    int inputLowValues[5];
    getCalibrationValues(inputLowValues);
    delay(100);

    for (int i = 0; i < 5; i = i + 1) {
      pinThresholds[i] = (inputHighValues[i] - inputLowValues[i]) / 2.5; 
    }

    Serial.println("Turning lasers ON again...");
    analogWrite(LASER_OUT_PIN, LASER_ON_PWM_VALUE);
  }

  bool checkMelody() {
    /* Checks melody against correctMelody */
    int i = 0;

    // melodyIndex is the index of the oldest note in melody, which acts as a ring buffer
    for (byte j = melodyIndex; j < 6; j = j + 1) {
      if (melody[j] != correctMelody[i]) {
        return false;
      }
      i++;
    }
    for (byte j = 0; j < melodyIndex; j = j + 1) {
      if (melody[j] != correctMelody[i]) {
        return false;
      }
      i++;
    }
    return true;
  }


  void midiSetInstrument(uint8_t chan, uint8_t inst) {
    if (chan > 15) return;
    inst --; // page 32 has instruments starting with 1 not 0 :(
    if (inst > 127) return;
    
    VS1053_MIDI.write(MIDI_CHAN_PROGRAM | chan);  
    VS1053_MIDI.write(inst);
  }

  void midiSetChannelVolume(uint8_t chan, uint8_t vol) {
    if (chan > 15) return;
    if (vol > 127) return;
    
    VS1053_MIDI.write(MIDI_CHAN_MSG | chan);
    VS1053_MIDI.write(MIDI_CHAN_VOLUME);
    VS1053_MIDI.write(vol);
  }

  void midiSetChannelBank(uint8_t chan, uint8_t bank) {
    if (chan > 15) return;
    if (bank > 127) return;
    
    VS1053_MIDI.write(MIDI_CHAN_MSG | chan);
    VS1053_MIDI.write((uint8_t)MIDI_CHAN_BANK);
    VS1053_MIDI.write(bank);
  }

  void midiNoteOn(uint8_t chan, uint8_t n, uint8_t vel) {
    if (chan > 15) return;
    if (n > 127) return;
    if (vel > 127) return;
    
    VS1053_MIDI.write(MIDI_NOTE_ON | chan);
    VS1053_MIDI.write(n);
    VS1053_MIDI.write(vel);
  }

  void midiNoteOff(uint8_t chan, uint8_t n, uint8_t vel) {
    if (chan > 15) return;
    if (n > 127) return;
    if (vel > 127) return;
    
    VS1053_MIDI.write(MIDI_NOTE_OFF | chan);
    VS1053_MIDI.write(n);
    VS1053_MIDI.write(vel);
  }
