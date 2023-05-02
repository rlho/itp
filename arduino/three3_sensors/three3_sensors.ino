#include "pitches.h"


const int threshold = 10;      // minimum reading of the sensors that generates a note
const int speakerPin = 4;      // pin number for the speaker
const int motorPin = 5;
const int noteDuration = 20;   // play notes for 20 ms

int notes[] = { NOTE_A4, NOTE_B4,NOTE_C3 };
int analogValue = 0;        // value read from the pot
int frequency = 0; 

void setup() {
 pinMode(motorPin, OUTPUT);
}
 
void loop() {
  for (int thisSensor = 0; thisSensor < 3; thisSensor++) {
    // get a sensor reading:
    int sensorReading = analogRead(thisSensor);
     Serial.println(thisSensor);
    // if the sensor is pressed hard enough:
    if (sensorReading > threshold) {
      // play the note corresponding to this sensor:
      tone(speakerPin, notes[thisSensor], noteDuration);
      digitalWrite(motorPin,HIGH);
    }
  }
 }
