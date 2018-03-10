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

void setup() {
    // All switches are output
	for (int ii = 0; ii < numSwitches; ii++) {
		pinMode(switches[ii],OUTPUT);
	}
}

void loop() {
    togglePin(switches[0], isSwitchOn[0]);
    delay(delayTime);
	togglePin(switches[0], isSwitchOn[0]);
    delay(delayTime);
	togglePin(switches[1], isSwitchOn[1]);
    delay(delayTime);
    togglePin(switches[1], isSwitchOn[1]);
    delay(delayTime);
    togglePin(switches[2], isSwitchOn[2]);
    delay(delayTime);
    togglePin(switches[2], isSwitchOn[2]);
    delay(delayTime);
}


/**
  Toggles a pin value from low to high based on a Boolean.

  @param pinIsHigh bool indicating current pin value. Changed when pin is toggled.
*/
void togglePin(int pin, bool &pinIsHigh)
{
    if (pinIsHigh) {
        digitalWrite(pin,LOW);
        pinIsHigh = false;
    }
    else {
        digitalWrite(pin,HIGH);
        pinIsHigh = true;
    }
}
