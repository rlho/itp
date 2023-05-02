// include library
#include "Adafruit_VL53L0X.h"
#include "Servo.h"

// make an instance of the library:
Adafruit_VL53L0X sensor = Adafruit_VL53L0X();
Servo servoMotor;       // creates an instance of the servo object to control a servo
int servoPin = 9;       // Control pin for servo motor
 
const int maxDistance  = 2000;
 
void setup() {
  // initialize serial, wait 3 seconds for
  // Serial Monitor to open:
  Serial.begin(9600);
  if (!Serial) delay(3000);
  
  servoMotor.attach(servoPin);
 
  // initialize sensor, stop if it fails:
  if (!sensor.begin()) {
    Serial.println("Sensor not responding. Check wiring.");
    while (true);
  }
  /* config can be:
    VL53L0X_SENSE_DEFAULT: about 500mm range
    VL53L0X_SENSE_LONG_RANGE: about 2000mm range
    VL53L0X_SENSE_HIGH_SPEED: about 500mm range
    VL53L0X_SENSE_HIGH_ACCURACY: about 400mm range, 1mm accuracy
  */
  sensor.configSensor(Adafruit_VL53L0X::VL53L0X_SENSE_LONG_RANGE);
  // set sensor to range continuously:
  sensor.startRangeContinuous();
}
void loop() {
  

  if (sensor.isRangeComplete()) {
    int result = sensor.readRangeResult();
    if (result < maxDistance) {
  
      int servoAngle = map(result*5, 0, 2000, 0, 360);
      servoMotor.write(servoAngle);
      Serial.println(result);
    }
  }
}
