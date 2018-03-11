// -----------------------------------------
//  Add sensing to laser
// -----------------------------------------
// This is basically the second half of the laser maze. 


// Declare pin variables

// Each switch is a signal for a MOSFET connected to 1 or 2 lasers
const int numSwitches = 3;
const int switches[numSwitches] = { D0, D1, D2 };

// Each photoresistor detects the prescence or absence of a laser
const int numLasers = 5;
const int photoresistors[numLasers] = { A0, A1, A2, A3, A4 };
int beamThresholds[numLasers];

// Controlling TIME
int blinkPeriod = 2000; // How long should each laser stay on for?
int timeSinceBlink = 0;
int loopTime = 1000;

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
	
	togglePin(switches[0]);
}

void loop() {
    char timeString[40];
    sprintf(timeString, "time since blink: %d", timeSinceBlink);
    Particle.publish("timeSinceBlink",timeString,60,PRIVATE);
    
    publishResistorReadings();
    
    if (timeSinceBlink >= blinkPeriod) {
        togglePin(switches[0]);
        timeSinceBlink = 0;
    }
    
    if(isBeamBroken()) {
        Particle.publish("alert","beam is broken!",60,PRIVATE);
    }

    digitalWrite(led, LOW);

    timeSinceBlink += loopTime;
    delay(loopTime);
    digitalWrite(led, HIGH);
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
	for (int ii = 0; ii < 1; ii++) {
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
  Publish a beamMeasurement event with the analog values of each photoresistor.
 */
void publishResistorReadings() {
    char publishString[128] = "";
    
    for (int ii = 0; ii < numLasers; ii++) {
        int pin = photoresistors[ii];
        sprintf(publishString + strlen(publishString), "pin %d: %d, ", pin, analogRead(pin));
	}
    
    Particle.publish("beamMeasurement", publishString, 60, PRIVATE);
}