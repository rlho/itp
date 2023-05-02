const int switchPin = 9;    // switch input
const int motor1Pin = 10;    // Motor driver leg 1 (pin 3, AIN1)
const int motor2Pin = 11;    // Motor driver leg 2 (pin 4, AIN2)
const int pwmPin = 12;       // Motor driver PWM pin

void setup() {
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
    // if the switch is high, motor will turn on one direction:
    if (digitalRead(switchPin) == HIGH) {
      
      Serial.println("high");
      if(digitalRead(pwmPin) == HIGH) {
        digitalWrite(pwmPin, LOW);
        delay(100);
      } else {
        digitalWrite(pwmPin, HIGH);
         delay(100);
      }
      
    }
    // if the switch is low, motor will turn in the other direction:
    else {
      Serial.println("low");
      //digitalWrite(pwmPin, LOW);
      //digitalWrite(motor1Pin, LOW);   // set leg 1 of the motor driver low
      }
  }
