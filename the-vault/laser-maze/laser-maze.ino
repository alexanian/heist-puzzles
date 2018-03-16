// -----------------------------------------
//  LASER MAZE
// -----------------------------------------
//
// Blink mode: The laser maze is set up to be navigated
// Alarm mode: The laser maze is flashing frantically!
//

// Declare pin variables

// Each switch is a signal for a MOSFET connected to 1 or 2 lasers
const int numSwitches = 3;
const int switches[numSwitches] = { D0, D1, D2 };

// Each photoresistor detects the prescence or absence of a laser
const int numLasers = 4;
const int photoresistors[numLasers] = { A0, A1, A2, A3 }; // one laser is a decoy
const int laserSwitches[numLasers] = { D2, D1, D2, D1 }; // some are doubled up
int beamThresholds[numLasers];

// Controlling TIME
int loopTime = 50;          // How often should we cycle through our loop?
int blinkPeriod = 1000;     // How long should each laser stay on for in blink mode?
int alarmPeriod = 10000;    // How long should the alarm stay on for? <- SEVEN MINS
int timePassed = 0;         // Tracking time since last alarm/blink cycle trigger

// Track whether to be in maze mode or alert mode
bool isAlarmGoingOff = false;

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
	
	// Publish calibration readings
	publishBeamThresholds();
	
	// Set just pin 2 high
	togglePin(switches[2]);
}

void loop() {
    if (isAlarmGoingOff) {
        if (timePassed < alarmPeriod) {
            // Continue PANIC MODE
            flashLasers();
        }
        else {
            // Finish panicking
            isAlarmGoingOff = false;
            timePassed = 0;
        }
    }
    else {
        // Normal blinking, not panicking at all
        if (isBeamBroken()) {
            // K no begin PANIC MODE
            isAlarmGoingOff = true;
            timePassed = 0;
            switchLasersOn();
            
            // Warn security
            Particle.publish("alert","beam is broken!",60,PRIVATE);
            publishResistorReadings();
        }
        else if (timePassed >= blinkPeriod) {
            // Blink the lasers :)
            publishResistorReadings();
            flashLasers();
            timePassed = 0;
        }
    }

    timePassed += loopTime;
    delay(loopTime);
}

/**
  Make all the lasers be on!
 */
void switchLasersOn() {
    for (int ii = 0; ii < numSwitches; ii++) {
		digitalWrite(switches[ii], HIGH);
	}
}

/**
  Make all the lasers be different!
 */
void flashLasers() {
       for (int ii = 0; ii < numSwitches; ii++) {
		togglePin(switches[ii]);
	}
}

/**
  Toggles a pin value between low and high.
  
  @param pin integer id of the pin to be toggled.
*/
void togglePin(int pin) {
    bool isPinHigh = digitalRead(pin);
    
    if (isPinHigh) {
        digitalWrite(pin, LOW);
    }
    else {
        digitalWrite(pin, HIGH);
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
  Publish a beamThreshold event with the calculated thresholds for each photoresistor.
 */
void publishBeamThresholds() {
    char publishString[128] = "";

    for (int ii = 0; ii < numLasers; ii++) {
        int pin = photoresistors[ii];
        sprintf(publishString + strlen(publishString), "beam %d: %d, ", pin, beamThresholds[ii]);
	}
    Particle.publish("beamThreshold", publishString, 60, PRIVATE);
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