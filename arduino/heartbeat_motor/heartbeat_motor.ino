#include <Wire.h>
#include "MAX30105.h"

#include "heartRate.h"
MAX30105 particleSensor;

const int switchPin = 9;    // switch input
const int motor1Pin = 10;    // Motor driver leg 1 (pin 3, AIN1)
const int motor2Pin = 11;    // Motor driver leg 2 (pin 4, AIN2)
const int pwmPin = 12;       // Motor driver PWM pin

const byte RATE_SIZE = 4; //Increase this for more averaging. 4 is good.
byte rates[RATE_SIZE]; //Array of heart rates
byte rateSpot = 0;
long lastBeat = 0; //Time at which the last beat occurred

float beatsPerMinute;
int beatAvg;

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


    //Heartbeat sensors
      Serial.begin(115200);
  Serial.println("Initializing...");

  // Initialize sensor
  if (!particleSensor.begin(Wire, I2C_SPEED_FAST)) //Use default I2C port, 400kHz speed
  {
    Serial.println("MAX30105 was not found. Please check wiring/power. ");
    while (1);
  }
  Serial.println("Place your index finger on the sensor with steady pressure.");

  particleSensor.setup(); //Configure sensor with default settings
  particleSensor.setPulseAmplitudeRed(0x0A); //Turn Red LED to low to indicate sensor is running
  particleSensor.setPulseAmplitudeGreen(0); //Turn off Green LED

  }

void loop() {

    // if the switch is high, motor will turn on one direction:
      long irValue = particleSensor.getIR();

  if (checkForBeat(irValue) == true)
  {
          //We sensed a beat!
    long delta = millis() - lastBeat;
    lastBeat = millis();

    beatsPerMinute = 60 / (delta / 1000.0);

    if (beatsPerMinute < 255 && beatsPerMinute > 20)
    {
      rates[rateSpot++] = (byte)beatsPerMinute; //Store this reading in the array
      rateSpot %= RATE_SIZE; //Wrap variable

      //Take average of readings
      beatAvg = 0;
      for (byte x = 0 ; x < RATE_SIZE ; x++)
        beatAvg += rates[x];
      beatAvg /= RATE_SIZE;
    }
  

      Serial.print("IR=");
      Serial.print(irValue);
      Serial.print(", BPM=");
      Serial.print(beatsPerMinute);
      Serial.print(", Avg BPM=");
      Serial.print(beatAvg);
      Serial.println();
      // generate caviar if I sensed a beat
      if (irValue > 50000) {
            Serial.println("high");
            digitalWrite(pwmPin, HIGH);
            delay(2000);
            digitalWrite(pwmPin, LOW);
      }
    }
  }
