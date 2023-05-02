#include "Servo.h"      // include the servo library
 
Servo servoMotor;       // creates an instance of the servo object to control a servo
int servoPin = 9;       // Control pin for servo motor
// time when the servo was last updated, in ms
long lastMoveTime = 0;  
 
void setup() {
  Serial.begin(9600);       // initialize serial communications
  servoMotor.attach(servoPin);  // attaches the servo on pin 9 to the servo object
} 
 
void loop() {

  int i = 0;
   for(i=0;i<=180;i++){
    servoMotor.write(i);
    delay(15);
  }for(i=0;i<=180;i++){
    servoMotor.write(180-i);
    delay(15);
  }

}
