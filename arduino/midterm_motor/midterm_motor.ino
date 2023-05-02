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


    
      digitalWrite(motor1Pin, LOW);   // set leg 1 of the motor driver low
      digitalWrite(motor2Pin, 1.2);
      delay(100);
      // set leg 2 of the motor driver high
}
