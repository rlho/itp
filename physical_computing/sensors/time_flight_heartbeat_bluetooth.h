// include library
#include "Adafruit_VL53L0X.h"
#include "Servo.h"
#include "MAX30105.h"
#include <ArduinoBLE.h>
#include "heartRate.h"
// Time-flight sensor
Adafruit_VL53L0X sensor0 = Adafruit_VL53L0X();

// Bluetooth
//  create service
BLEService duplexService("473da924-c93a-11e9-a32f-2a2ae2dbcce4");
// create button characteristic and allow remote device to get notifications
BLEByteCharacteristic buttonCharacteristic("473da924-c93a-11e9-a32f-2a2ae2dbcce3",
                                           BLERead | BLENotify);
// create LED characteristic and allow remote device to read and write
BLEByteCharacteristic ledCharacteristic("473da924-c93a-11e9-a32f-2a2ae2dbcce4",
                                        BLERead | BLEWrite);

const int ledPin = LED_BUILTIN; // set ledPin to on-board LED

// Heartbeat
MAX30105 particleSensor;

// Control multiple sensors

int sensor2ShutdownPin = 3;

void setup()
{
  // initialize serial, wait 3 seconds for
  // Serial Monitor to open:
  Serial.begin(9600);
  if (!Serial)
    delay(3000);

  // Initialize sensor
  pinMode(ledPin, OUTPUT); // use the LED as an output

  pinMode(sensor2ShutdownPin, OUTPUT);

  digitalWrite(sensor2ShutdownPin, LOW);

  delay(10);

  digitalWrite(sensor2ShutdownPin, HIGH);
  digitalWrite(sensor2ShutdownPin, LOW);

  // Bluetooth
  if (!BLE.begin())
  {
    Serial.println("starting Bluetooth® Low Energy module failed!");
    while (1)
      ;
  }
  // set the local name peripheral advertises
  BLE.setDeviceName("TimeFlightHeartBeat");
  BLE.setAdvertisedService(duplexService);
  // add the characteristics to the service
  duplexService.addCharacteristic(ledCharacteristic);
  duplexService.addCharacteristic(buttonCharacteristic);

  // add the service
  BLE.addService(duplexService);
  ledCharacteristic.writeValue(0);
  buttonCharacteristic.writeValue(0);
  // start advertising
  BLE.advertise();
  Serial.println("Bluetooth® device active, waiting for connections...");

  // Heartbeat Sensor
  if (!particleSensor.begin(Wire)) // Use default I2C port, 400kHz speed
  {
    Serial.println("MAX30105 was not found. Please check wiring/power. ");
    while (1)
      ;
  }

  particleSensor.setup();                    // Configure sensor with default settings
  particleSensor.setPulseAmplitudeRed(0x0A); // Turn Red LED to low to indicate sensor is running
  particleSensor.setPulseAmplitudeGreen(0);  // Turn off Green LED

  // Time-Flight
  digitalWrite(sensor2ShutdownPin, HIGH);
  // initialize sensor, stop if it fails:
  if (!sensor0.begin(0x29))
  {
    Serial.println("Sensor0 not responding. Check wiring.");
    while (true)
      ;
  }
  /* config can be:
    VL53L0X_SENSE_DEFAULT: about 500mm range
    VL53L0X_SENSE_LONG_RANGE: about 2000mm range
    VL53L0X_SENSE_HIGH_SPEED: about 500mm range
    VL53L0X_SENSE_HIGH_ACCURACY: about 400mm range, 1mm accuracy
  */
  sensor0.configSensor(Adafruit_VL53L0X::VL53L0X_SENSE_LONG_RANGE);
  // set sensor to range continuously:
  sensor0.startRangeContinuous();
  Serial.println("started!");
}
void loop()
{

  // poll for Bluetooth® Low Energy events
  BLEDevice central = BLE.central();
  if (central)
  {
    Serial.println("Got a central");
    while (central.connected())
    {
      if (sensor0.isRangeComplete())
      {
        int result0 = sensor0.readRangeResult();
        Serial.print("result0: ");
        ledCharacteristic.writeValue(result0);
        Serial.println(result0);
      }
      long irValue = particleSensor.getIR();

      if (checkForBeat(irValue) == true)
      {
        buttonCharacteristic.writeValue(true);
        Serial.println("blink");
      }
      else
      {
        buttonCharacteristic.writeValue(false);
      }
    }
  }
  // when the central disconnects, print it out:
  Serial.print("Disconnected from central: ");
  Serial.println(central.address());
}