const int switchPin = 2;    // switch input
const int motor1Pin = 3;    // Motor driver leg 1 (pin 3, AIN1)
const int motor2Pin = 4;    // Motor driver leg 2 (pin 4, AIN2)
const int pwmPin = 5;       // Motor driver PWM pin


void setup() {
  // put your setup code here, to run once:
    // set the switch as an input:
    pinMode(switchPin, INPUT); 
 
    // set all the other pins you're using as outputs:
    pinMode(motor1Pin, OUTPUT);
    pinMode(motor2Pin, OUTPUT);
    pinMode(pwmPin, OUTPUT);
 
    // set PWM enable pin high so that motor can turn on:
    digitalWrite(pwmPin, HIGH);
}

void loop() {
  // put your main code here, to run repeatedly:
    // if the switch is high, motor will turn on one direction:
    if (digitalRead(switchPin) == HIGH) {
      digitalWrite(motor1Pin, LOW);   // set leg 1 of the motor driver low
      digitalWrite(motor2Pin, HIGH);  // set leg 2 of the motor driver high
    }
    // if the switch is low, motor will turn in the other direction:
    else {
      digitalWrite(motor1Pin, HIGH);  // set leg 1 of the motor driver high
      digitalWrite(motor2Pin, LOW);   // set leg 2 of the motor driver low
    }
}
