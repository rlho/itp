// Programmable-Air
// Author: tinkrmind
// https://github.com/orgs/Programmable-Air
// 
// Basic example of using Vent function of the Programmable-Air board. 
// The neopixels show pressure. Cyan for low pressure and Purple for high pressure
// Brighter color means more extreme pressure. Bright cyan means lower pressure than dim cyan and brighter purple means higher pressure tham dim purple. At atmospheric presure,lights will be off.
//
// PCB v0.3/v0.4/v0.5

#include "programmable_air.h"

#define DEBUG 1

const int trigPin = 8;
const int echoPin = 9;

void setup() {
  initializePins();
   pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  // initialize serial communication:
  Serial.begin(9600);
}

void loop() {

   // take the trigger pin low to start a pulse:
  digitalWrite(trigPin, LOW);
  // delay 2 microseconds:
  delayMicroseconds(2);
  // take the trigger pin high:
  digitalWrite(trigPin, HIGH);
  // delay 10 microseconds:
  delayMicroseconds(10);
  // take the trigger pin low again to complete the pulse:
  digitalWrite(trigPin, LOW);
 
  // listen for a pulse on the echo pin:
  long duration = pulseIn(echoPin, HIGH);
  // calculate the distance in cm.
  //Sound travels approx.0.0343 microseconds per cm.,
  // and it's going to the target and back (hence the /2):
  int distance = (duration * 0.0343) / 2;
  Serial.print("Distance: ");
  Serial.println(distance);
  // a short delay between readings:
  delay(10);

  
  // if the RED button is pressed blow air into the output, else, vent to atmosphere
  if(distance<30){
    // switch on Pump 2 (blow) to 50% duty cycle
    switchOnPump(2,100-distance);
    blow();
  } else{
    switchOffPumps();
    vent();
  }
  if(readBtn(RED)){
    switchOffPumps();
    vent();
  }
  }
}
