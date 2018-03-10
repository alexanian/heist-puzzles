// -----------------------------------------
//  Blink lasers according to API timeouts 
// -----------------------------------------
// This is basically the first half of the laser mase. 


// Declare pin variables
int switch1 = D0; // Signal for a MOSFET connected to Laser 1
int switch2 = D1; // Signal for a MOSFET connected to Laser 2 and Laser 3
int switch3 = D2; // Signal for a MOSFET connected to Laser 4 and Laser 5

int photoresistor = A1;


// Booleans to indicate current status of each laser
bool isLaser1 = false;
bool isLaser2 = false;
bool isLaser3 = false;
bool isLaser4 = false;
bool isLaser5 = false;

char publishString[40];
int read;

void setup() {
    // All switches are output
    pinMode(switch1,OUTPUT);
    pinMode(switch2,OUTPUT);
    pinMode(switch3,OUTPUT);
    
    digitalWrite(switch1,HIGH);
    digitalWrite(switch2,HIGH);
    digitalWrite(switch3,HIGH);
}

void loop() {
    toggleSwitchOne();
    read = analogRead(photoresistor);
    sprintf(publishString, "%d", read);
    Particle.publish("beamMeasurement",publishString,60,PRIVATE);
    delay(1000);
    toggleSwitchOne();
    read = analogRead(photoresistor);
    sprintf(publishString, "%d", read);
    Particle.publish("beamMeasurement",publishString,60,PRIVATE);
    delay(1000);
    toggleSwitchTwo();
    read = analogRead(photoresistor);
    sprintf(publishString, "%d", read);
    Particle.publish("beamMeasurement",publishString,60,PRIVATE);
    delay(1000);
    toggleSwitchTwo();
    read = analogRead(photoresistor);
    sprintf(publishString, "%d", read);
    Particle.publish("beamMeasurement",publishString,60,PRIVATE);
    delay(1000);
    toggleSwitchThree();
    read = analogRead(photoresistor);
    sprintf(publishString, "%d", read);
    Particle.publish("beamMeasurement",publishString,60,PRIVATE);
    delay(1000);
    toggleSwitchThree();
    read = analogRead(photoresistor);
    sprintf(publishString, "%d", read);
    Particle.publish("beamMeasurement",publishString,60,PRIVATE);
    delay(1000);
}


void toggleSwitchOne()
{
    if (isLaser1) {
        digitalWrite(switch1,LOW);
        isLaser1 = false;
    }
    else {
        digitalWrite(switch1,HIGH);
        isLaser1 = true;
    }
}

void toggleSwitchTwo()
{
    if (isLaser2) {
        digitalWrite(switch2,LOW);
        isLaser2 = false;
        isLaser3 = false;
    }
    else {
        digitalWrite(switch2,HIGH);
        isLaser2 = true;
        isLaser3 = true;
    }
}

void toggleSwitchThree()
{
    if (isLaser4) {
        digitalWrite(switch3,LOW);
        isLaser4 = false;
        isLaser5 = false;
    }
    else {
        digitalWrite(switch3,HIGH);
        isLaser4 = true;
        isLaser5 = true;
    }
}