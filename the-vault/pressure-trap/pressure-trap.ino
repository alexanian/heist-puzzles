// Heavily drawing from: http://www.instructables.com/id/IoT-Scale/
// This #include statement was automatically added by the Particle IDE.
#include <HX711ADC.h>
#include <neopixel.h>

// HX711 Pin mapping
#define SCK A3
#define DT A2

// Set LED count, data pin ID and strip type
#define LED_COUNT 60
#define PIXEL_PIN D1
#define PIXEL_TYPE WS2812B

// Initialize the LED object 
Adafruit_NeoPixel strip = Adafruit_NeoPixel(LED_COUNT, PIXEL_PIN, PIXEL_TYPE);

// Initialize the scale hardware
HX711ADC scale(DT,SCK);

// Track whether to be in maze mode or alert mode
bool isAlarmGoingOff = false;

// Timing variables
const unsigned long loopTime = 10;          // How often should we cycle through our loop (governs response time)?
const unsigned long lightingRate = 20;     // How fast should the happy LEDs flow
unsigned long lightingTrigger = millis();
const unsigned long flashRate = 166.6;        // How fast should the panic LEDs flash
unsigned long flashTrigger = millis();

// Scale variables
double prevScaleValue;
double scaleValue;
const double scaleTrigger = 5000;           // How large a delta from reading to reading should trigger an alarm 
const unsigned long scaleSampleRate = 1000;  // How frequently should we read the scale? 
unsigned long scaleTimeTrigger = millis();  

// Functions to provide
int tareScale(String command);
double absValue(double value);

// SYNTAX
bool success = Particle.function("reset", resetAlarm);

void setup(){
    // Declare Particle variables accessible from the Cloud
    Particle.variable("scaleValue", scaleValue);
    Particle.function("tareScale", tareScale);
    
    // Calibrate scale; instructions from: https://github.com/juano2310/HX711ADC
    // 1. Call set_scale() with no parameter.
    scale.set_scale();
    // 2. Call tare() with no parameter
    scale.tare();
    // 3. Place a known weight on the scale and call get_units(10)
    Particle.publish("calibrating", "Place object with known weight on the scale");
    double knownWeight = 163; // Using the weight of my phone (163g)
    delay(3000);
    double reading = scale.get_units(10);
    // 4. Divide the result in step 3 to your known weight. You should get about the parameter you need to pass to set_scale.
    double scalingFactor = reading / knownWeight; 
    
    Particle.publish("calibrating", "Remove object from the scale");
      
    char publishString[128] = "";
    sprintf(publishString, "scaling factor is: %f", scalingFactor);
    Particle.publish("scalingFactor", publishString);
    delay(3000);
      
    // 5. Adjust the parameter in step 4 until you get an accurate reading.
    scale.set_scale(scalingFactor);
    
    scale.tare();
    scaleValue = scale.get_units(5);
    
    // Initialize LEDs
    strip.begin();
    strip.show();
}


// For-ever loop
void loop(){
    // Record the current time for looping/triggering purposes
    unsigned long currentTime = millis();
    
    if (isAlarmGoingOff && (currentTime > flashTrigger)) {
        alarmLighting();
        flashTrigger = currentTime + flashRate;
    }
    
    if (!isAlarmGoingOff && (currentTime > lightingTrigger)) {
        happyLighting();
        lightingTrigger = currentTime + lightingRate;
    }
    
    if (currentTime > scaleTimeTrigger) {
        measure_weight();
        scaleTimeTrigger = currentTime + scaleSampleRate;
    }
}


// Weight measurement 
void measure_weight(){
    scale.power_up();

    // Check for a change in weight and send event
    if ( scaleValue > prevScaleValue + 2 || scaleValue < prevScaleValue - 2) {
        char publishString[128] = "";
        sprintf(publishString, "scale value is: %f", scaleValue);
        Particle.publish("weightChange", publishString);
    }
    
    // Check whether the change from one reading to the next should cause an alarm
    if (abs(scaleValue - prevScaleValue) > scaleTrigger){
        isAlarmGoingOff = true;
        Particle.publish("alert","item disturbed!", 60, PRIVATE);
    }

    prevScaleValue = scaleValue;
    scaleValue = scale.get_units(5);
    scaleValue = abs(scaleValue);

    scale.power_down();
    
}


// Scale tare function
int tareScale(String command) {
    scale.tare();
    delay(100);
    Serial.println("Scale Tared From Cloud!!");
    return 1;
}


// Alarm lighting function
void alarmLighting(){
    static bool toggle = true;
    if (toggle){
        for (uint8_t i = 0; i < LED_COUNT; i++) {
            strip.setPixelColor(i, 200,0,0);
        } 
        toggle = false;
    } 
    else{
        for (uint8_t i = 0; i < LED_COUNT; i++) {
            strip.setPixelColor(i, 0,0,0);
        } 
        toggle = true;
    }
    strip.show();
}


// Everything's happy lighting function
void happyLighting(){
    static int startPixel = 0;
    static int cycleStart = 0;
    
    startPixel = cycleStart;
    
    for (uint8_t i = 0; i < LED_COUNT; i++) {
        
        if (i <= LED_COUNT/2){
            strip.setPixelColor(startPixel, 0,(150-(i*5)),(0+(i*7)));
        }
        
        if (i > LED_COUNT/2){
            strip.setPixelColor(startPixel, 0,(0+(i*5)),(210-(i*7)));
        }
        
        if (startPixel < LED_COUNT){
            startPixel++;        
        }
        else{
            startPixel = 0;
        }
    }
        
    strip.show();
    cycleStart++;
    
}

// Alarm reset
int resetAlarm(String dummy){
    isAlarmGoingOff = false;   
    return 0;
}