// -----------------------------------------
//  Add sensing to laser
// -----------------------------------------
// This is basically the second half of the laser maze. 


// Declare pin variables

// Each switch is a signal for a MOSFET connected to 1 or 2 lasers
const int numSwitches = 3;
const int switches[numSwitches] = { D0, D1, D2 };

// Each photoresistor detects the prescence or absence of a laser
const int numLasers = 4;
const int photoresistors[numLasers] = { A0, A1, A2, A3 };
const int laserSwitches[numLasers] = { D2, D1, D2, D1 }; // some are doubled up
int beamThresholds[numLasers];

// Controlling TIME
int blinkPeriod = 1000; // How long should each laser stay on for?
int timeSinceBlink = 0;
int loopTime = 500;

void setup() {
    // All switches are output
	for (int ii = 0; ii < numSwitches; ii++) {
		pinMode(switches[ii],OUTPUT);
		digitalWrite(switches[ii], LOW);
	}
	
	// Calibrate photoresistors
	for (int ii = 0; ii < numLasers; ii++) {
	    beamThresholds[ii] = calibrateResistor(photoresistors[ii], laserSwitches[ii]);
	}
	
    char publishString[128] = "";
    for (int ii = 0; ii < numLasers; ii++) {
        int pin = photoresistors[ii];
        sprintf(publishString + strlen(publishString), "beam %d: %d, ", pin, beamThresholds[ii]);
	}
    Particle.publish("beamThreshold", publishString, 60, PRIVATE);
	
	togglePin(switches[2]);
	togglePin(switches[1]);
    togglePin(switches[0]);
}

void loop() {
    if(isBeamBroken()) {
        publishResistorReadings();
        Particle.publish("alert","beam is broken!",60,PRIVATE);
    }
    else if (timeSinceBlink >= blinkPeriod) {
        publishResistorReadings();
        togglePin(switches[2]);
    	togglePin(switches[1]);
    	togglePin(switches[0]);
        timeSinceBlink = 0;
    }
    
    timeSinceBlink += loopTime;
    delay(loopTime);
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
  Calibrate photoresistors- necessary because ambient light levels change.
  
  @param photoresistor integer id of photoresistor to read.
  @param laserSwitch integer id of switch for laser pointed at photoresistor.
 */
int calibrateResistor(int photoresistor, int laserSwitch) {
    // Average two measurements while the laser is on
    digitalWrite(laserSwitch, HIGH);
    int on_1 = analogRead(photoresistor);
    delay(1000);
    int on_2 = analogRead(photoresistor);
    delay(1000);
    int on_3 = analogRead(photoresistor);
    int on = (on_1 + on_2 + on_3) / 3;
    
    // Average two measurements while the laser if off
    digitalWrite(laserSwitch, LOW);
    int off_1 = analogRead(photoresistor);
    delay(1000);
    int off_2 = analogRead(photoresistor);
    delay(1000);
    int off_3 = analogRead(photoresistor);
    int off = (off_1 + off_2 + off_3) / 3;
    
    // Calibrate the beam threshold to be halfway between the 
    return (on*2 + off)/3;
}

/**
  Return true if any of the beams are broken.
*/
bool isBeamBroken() {
    bool isBeamBroken = false;
	for (int ii = 0; ii < numLasers; ii++) {
	    if (laserSwitchedOff(ii)) continue;
	    
	    int beamValue = analogRead(photoresistors[ii]);
	    isBeamBroken = isBeamBroken || ( beamValue < beamThresholds[ii] ); 
	}
	return isBeamBroken;
}


/**
  Return true if the switch controlling the laser is off.
*/
bool laserSwitchedOff(int laserNumber)
{
    return !digitalRead(laserSwitches[laserNumber]);
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