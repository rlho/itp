// include library
#include "Adafruit_VL53L0X.h"
#include "Servo.h"

// make an instance of the library:
Adafruit_VL53L0X sensor0 = Adafruit_VL53L0X();
Adafruit_VL53L0X sensor1 = Adafruit_VL53L0X();

Servo servoMotor;       // creates an instance of the servo object to control a servo
int servoPin = 9;       // Control pin for servo motor
int sensor1ShutdownPin = 2;
int sensor2ShutdownPin = 3;
 
const int maxDistance  = 2000;
 
void setup() {
  // initialize serial, wait 3 seconds for
  // Serial Monitor to open:
  Serial.begin(9600);
  if (!Serial) delay(3000);
  
  servoMotor.attach(servoPin);


  pinMode(sensor1ShutdownPin, OUTPUT);
  pinMode(sensor2ShutdownPin, OUTPUT);

 digitalWrite(sensor1ShutdownPin, LOW);
 digitalWrite(sensor2ShutdownPin, LOW);

 delay(10);

 digitalWrite(sensor1ShutdownPin, HIGH);
 digitalWrite(sensor2ShutdownPin, HIGH);
 digitalWrite(sensor2ShutdownPin, LOW);


  // initialize sensor, stop if it fails:
  if (!sensor0.begin(0x29)) {
    Serial.println("Sensor0 not responding. Check wiring.");
    while (true);
  }

 digitalWrite(sensor2ShutdownPin, HIGH);
 
  // initialize sensor, stop if it fails:
  if (!sensor1.begin(0x2A)) {
    Serial.println("Sensor1 not responding. Check wiring.");
    while (true);
  }

  /* config can be:
    VL53L0X_SENSE_DEFAULT: about 500mm range
    VL53L0X_SENSE_LONG_RANGE: about 2000mm range
    VL53L0X_SENSE_HIGH_SPEED: about 500mm range
    VL53L0X_SENSE_HIGH_ACCURACY: about 400mm range, 1mm accuracy
  */
  sensor0.configSensor(Adafruit_VL53L0X::VL53L0X_SENSE_LONG_RANGE);
  sensor1.configSensor(Adafruit_VL53L0X::VL53L0X_SENSE_LONG_RANGE);
  // set sensor to range continuously:
  sensor0.startRangeContinuous();
  sensor1.startRangeContinuous();

}
void loop() {
  if (sensor0.isRangeComplete()) {
    int result0 = sensor0.readRangeResult();
      Serial.print("result0: ");

      Serial.println(result0);
    if (result0 < maxDistance) {
  
      int servoAngle = map((result0)*5, 0, 2000, 0, 360);
      servoMotor.write(servoAngle);


    }
  }

  if (sensor1.isRangeComplete()) {

    int result1 = sensor1.readRangeResult();
    if (result1 < maxDistance) {
  
      int servoAngle = map((result1)*5, 0, 2000, 0, 360);
      servoMotor.write(servoAngle);
      //Serial.print("result1: ");

      //Serial.println(result1);
    }
  }


}
