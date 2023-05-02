// Programmable-Air
// Author: tinkrmind
// https://github.com/orgs/Programmable-Air
// 
// Basic example of using Vent function of the Programmable-Air board. 
// The neopixels show pressure. Cyan for low pressure and Purple for high pressure
// Brighter color means more extreme pressure. Bright cyan means lower pressure than dim cyan and brighter purple means higher pressure tham dim purple. At atmospheric presure,lights will be off.
//
// PCB v0.3/v0.4/v0.5


#include "MAX30105.h"
#include "Adafruit_VL53L0X.h"
#include "heartRate.h"
#define DEBUG 1
#define debug Serial
//Heartbeat
MAX30105 particleSensor;

void setup() {
  
  Serial.begin(9600);
  Serial.println("MAX30105 Basic Readings Example");

  // Initialize sensor
  if (particleSensor.begin() == false)
  {
    Serial.println("MAX30105 was not found. Please check wiring/power. ");
    while (1);
  }

  particleSensor.setup(); //Configure sensor. Use 6.4mA for LED drive
Serial.println("started!");


}

void loop() {



  long irValue = particleSensor.getIR();
  // if the RED button is pressed blow air into the output, else, vent to atmosphere
    if (checkForBeat(irValue) == true){
    // switch on Pump 2 (blow) to 50% duty cycle
    Serial.println("blink");
    //delay(200);
  } else{
    //switchOffPumps();
  }
  
}
