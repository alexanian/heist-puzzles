// -----------------------------------------
//  Add sensing to laser
// -----------------------------------------
// This is basically the second half of the laser maze. 


// Declare pin variables

// Each switch is a signal for a MOSFET connected to 1 or 2 lasers
int numSwitches = 3;
int switches[] = { D0, D1, D2 };

// Each photoresistor detects the prescence or absence of a laser
int numLasers = 5;
int photoresistors[] = { A0, A1, A2, A3, A4 };
int beamThresholds[5];

// Controlling TIME
int blinkPeriod = 1000; // How long should each laser stay on for?
int timeSinceBlink = 0;
int loopTime = 10;

// For publishing event strings!
char publishString[40];

void setup() {
    // All switches are output
	for (int ii = 0; ii < numSwitches; ii++) {
		pinMode(switches[ii],OUTPUT);
		digitalWrite(switches[ii], LOW);
	}
	
	// Calibrate photoresistors
	for (int ii = 0; ii < numLasers; ii++) {
	    beamThresholds[ii] = calibrateResistor(photoresistors[ii]);
	}
}

void loop() {
    publishResistorReadings();
    
    if(isBeamBroken()) {
        Particle.publish("alert","beam is broken!",60,PRIVATE);
    }
    else if (timeSinceBlink >= blinkPeriod) {
        // switch to next state
        togglePin(switches[1]);
        timeSinceBlink = 0;
    }

    timeSinceBlink += loopTime;
    delay(loopTime);
}


/**
  Right now, set the beam threshold for all lasers at 3800.
 */
int calibrateResistor(int pin) {
    return 3800;
}

/**
  Return true if any of the beams are broken
*/
bool isBeamBroken() {
    bool isBeamBroken = false;
	for (int ii = 0; ii < numLasers; ii++) {
	    int beamValue = analogRead(photoresistors[ii]);
	    isBeamBroken = isBeamBroken || beamValue < beamThresholds[ii]; 
	}
	return isBeamBroken;
}


/**
  Toggles a pin value between low and high.
  
  @param pin integer id of the pin to be toggled.
*/
void togglePin(int pin) {
    bool isPinHigh = digitalRead(pin);
    
    if (isPinHigh) {
        digitalWrite(pin,LOW);
    }
    else {
        digitalWrite(pin,HIGH);
    }
}

/**
  Publish readings for each resistor
 */
void publishResistorReadings() {
	for (int ii = 0; ii < numLasers; ii++) {
	    publishResistorReading(photoresistors[ii]);
	}
}

/**
  Publish a beamMeasurement event with the analog value of a pin.
  
  @param pin integer id of the pin to be read.
 */
void publishResistorReading(int pin) {
    int read = analogRead(pin);
    sprintf(publishString, "pin %d has value: %d", pin, read);
    Particle.publish("beamMeasurement",publishString,60,PRIVATE);
}