// -----------------------------------------
//  Blink lasers according to API timeouts 
// -----------------------------------------
// This is basically the first half of the laser maze. 


// Declare pin variables

// Each switch is a signal for a MOSFET connected to 1 or 2 lasers
int numSwitches = 3;
int switches[] = { D0, D1, D2 };

// Track whether switches are on
bool isSwitchOn[] = { false, false, false };

// How long should each laser stay on for?
int delayTime = 1000;

// For reading!
char publishString[40];

void setup() {
    // All switches are output
	for (int ii = 0; ii < numSwitches; ii++) {
		pinMode(switches[ii],OUTPUT);
	}
}

void loop() {
    rotateBlink(1000);
}


/**
  Toggles a pin value from low to high.

  @param pinIsHigh bool indicating current pin value. Inverted when pin is written.
*/
void togglePin(int pin, bool &pinIsHigh) {
    if (pinIsHigh) {
        digitalWrite(pin,LOW);
        pinIsHigh = false;
    }
    else {
        digitalWrite(pin,HIGH);
        pinIsHigh = true;
    }
}

/**
  Rotate having a single switch off.
  
  @param blinkTime milliseconds to keep each switch will be off for.
*/
void rotateBlink(int blinkTime) {
    
    // Setup: turn off and other two on
    digitalWrite(switches[0],LOW);
    isSwitchOn[0] = false;
    
    digitalWrite(switches[1],HIGH);
    isSwitchOn[1] = true;
    
    digitalWrite(switches[2],HIGH);
    isSwitchOn[2] = true;
    
    publishSwitchState();
    
    // Turn one off, turn zero on
    togglePin(switches[1], isSwitchOn[1]);
    togglePin(switches[0], isSwitchOn[0]);

    publishSwitchState();
    delay(blinkTime);
    
    // Turn two off, turn one on
    togglePin(switches[2], isSwitchOn[2]);
    togglePin(switches[1], isSwitchOn[1]);
    
    publishSwitchState();
    delay(blinkTime);
}

/**
  Publish a "switchState" event, i.e. if only switch two is on "010".
  Useful for debugging.
*/
void publishSwitchState() {
    sprintf(publishString, "%d %d %d",
        digitalRead(switches[0]),
        digitalRead(switches[1]),
        digitalRead(switches[2]));
    Particle.publish("switchState", publishString, 60, PRIVATE);
}
