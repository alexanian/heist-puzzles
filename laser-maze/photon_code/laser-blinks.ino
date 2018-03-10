// -----------------------------------------
//  Blink lasers according to API timeouts 
// -----------------------------------------
// This is basically the first half of the laser maze. 


// Declare pin variables

// Each switch is a signal for a MOSFET connected to 1 or 2 lasers
int numSwitches = 3;
int switches[] = { D0, D1, D2 };

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
  Toggles a pin value between low and high.
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
  Rotate having a single switch off.
*/
void rotateBlink(int blinkTime) {
    
    // Setup: turn off and other two on
    digitalWrite(switches[0],LOW);
    digitalWrite(switches[1],HIGH);
    digitalWrite(switches[2],HIGH);
    
    publishSwitchState();
    
    // Turn one off, turn zero on
    togglePin(switches[1]);
    togglePin(switches[0]);

    publishSwitchState();
    delay(blinkTime);
    
    // Turn two off, turn one on
    togglePin(switches[2]);
    togglePin(switches[1]);
    
    publishSwitchState();
    delay(blinkTime);
}


void publishSwitchState() {
    sprintf(publishString, "%d %d %d",
        digitalRead(switches[0]),
        digitalRead(switches[1]),
        digitalRead(switches[2]));
    Particle.publish("switchState", publishString, 60, PRIVATE);
}


/**
  Flash lasers very quickly. PANIC!!!
*/
void panic() {
}
