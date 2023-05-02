#include <CapacitiveSensor.h>

const int switchPin = 9;    // switch input
const int motor1Pin = 10;    // Motor driver leg 1 (pin 3, AIN1)
const int motor2Pin = 11;    // Motor driver leg 2 (pin 4, AIN2)
const int pwmPin = 12;       // Motor driver PWM pin
CapacitiveSensor   cs = CapacitiveSensor(7,6); 
void setup() {
    cs.set_CS_AutocaL_Millis(0xFFFFFFFF);
    Serial.begin(9600);
  
    // set the switch as an input:
    pinMode(switchPin, INPUT); 
 
    // set all the other pins you're using as outputs:
    pinMode(motor1Pin, OUTPUT);
    pinMode(motor2Pin, OUTPUT);
    pinMode(pwmPin, OUTPUT);
    digitalWrite(motor1Pin, HIGH);  // set leg 1 of the motor driver high
    analogWrite(motor2Pin, LOW);
    // set PWM enable pin high so that motor can turn on:
    digitalWrite(pwmPin, LOW);

  }

void loop() {
    long total1 =  cs.capacitiveSensor(200);
    Serial.println(total1);
    // if the switch is high, motor will turn on one direction:
    int a = digitalRead(switchPin);
    Serial.println(a);
    if (digitalRead(switchPin) == HIGH || total1 > 10000) {
      
        Serial.println("touch");
        digitalWrite(pwmPin, HIGH);
        delay(2000);
        digitalWrite(pwmPin, LOW);
    }
  }
