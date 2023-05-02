// include library
#include "Adafruit_VL53L0X.h"
#include "Servo.h"
#include "MAX30105.h"
#include <ArduinoBLE.h>
#include "heartRate.h"
//Time-flight sensor
Adafruit_VL53L0X sensor0 = Adafruit_VL53L0X();

//Bluetooth
BLECharacteristic ledCharacteristic;
BLECharacteristic buttonCharacteristic;

// peripheral characteristic flags:
bool ledAvailable = false;
bool buttonSubscribed = false;
const int ledPin = LED_BUILTIN; // set ledPin to on-board LED

//Heartbeat
MAX30105 particleSensor;

//Control multiple sensors

int sensor2ShutdownPin = 3;
 
void setup() {
  // initialize serial, wait 3 seconds for
  // Serial Monitor to open:
  Serial.begin(9600);
  if (!Serial) delay(3000);

  // Initialize sensor
  pinMode(ledPin, OUTPUT); // use the LED as an output

  pinMode(sensor2ShutdownPin, OUTPUT);

  digitalWrite(sensor2ShutdownPin, LOW);

  delay(10);


  digitalWrite(sensor2ShutdownPin, HIGH);
  digitalWrite(sensor2ShutdownPin, LOW);
 
  // initialize the BLE hardware
  BLE.begin();
    

  // start scanning for peripherals
  BLE.scanForUuid("473da924-c93a-11e9-a32f-2a2ae2dbcce4");
  Serial.println("central is scanning");

  
  //Heartbeat Sensor
  if (!particleSensor.begin(Wire)) //Use default I2C port, 400kHz speed
  {
    Serial.println("MAX30105 was not found. Please check wiring/power. ");
    while (1);
  }
  particleSensor.setup(); //Configure sensor with default settings
  particleSensor.setPulseAmplitudeRed(0x0A); //Turn Red LED to low to indicate sensor is running
  particleSensor.setPulseAmplitudeGreen(0); //Turn off Green LED

  //Time-Flight
  digitalWrite(sensor2ShutdownPin, HIGH);
  // initialize sensor, stop if it fails:
  if (!sensor0.begin(0x29)) {
    Serial.println("Sensor0 not responding. Check wiring.");
    while (true);
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

}
void loop() {


    // check if a peripheral has been discovered
  BLEDevice peripheral = BLE.available();

  if (peripheral) {
    Serial.println("Got a peripheral");
    // peripheral detected, see if it's the right one:

    if (peripheral.advertisedServiceUuid() == "473da924-c93a-11e9-a32f-2a2ae2dbcce4") {
      Serial.println("Got the right peripheral");
      // If so, stop scanning and start communicating with it:
      BLE.stopScan();
      communicateWith(peripheral);
    }
    // when the peripheral disconnects, resume scanning:
    // reset the peripheral characteristic flags:
    ledAvailable = false;
    buttonSubscribed = false;
    // start scanning again:
    BLE.scanForUuid("473da924-c93a-11e9-a32f-2a2ae2dbcce4");
  }

  
}


void communicateWith(BLEDevice peripheral) {
  // connect to the peripheral
  Serial.println("Connecting ...");
  // if you can't connect, go back to the main loop:
  if (!peripheral.connect()) {
    Serial.println("Can't connect");
    return;
  }
  // If you can't discover peripheral attributes
  // go back to the main loop:
  if (!peripheral.discoverAttributes()) {
    Serial.println("Didn't discover attributes");
    peripheral.disconnect();
    return;
  }


  // when you know the UUIDs of the characteristics you want,
  // you can just pull them out of the remote peripheral like this:
  buttonCharacteristic = peripheral.characteristic("473dab7c-c93a-11e9-a32f-2a2ae2dbcce4");
  ledCharacteristic = peripheral.characteristic("473dacc6-c93a-11e9-a32f-2a2ae2dbcce4");

  if (buttonCharacteristic.canSubscribe()) {
    Serial.println("can subscribe");
    if (buttonCharacteristic.subscribe()) {
      Serial.println("button subscribed");
    }
  }
  // retrieve the LED characteristic
  BLECharacteristic ledCharacteristic = peripheral.characteristic("473dacc6-c93a-11e9-a32f-2a2ae2dbcce4");

  if(ledCharacteristic) {
      if (sensor0.isRangeComplete()) {
        char result0 = sensor0.readRangeResult();
      Serial.print("result0: ");
      ledCharacteristic.writeValue("2");
      Serial.println(result0);
      
      }
  }
 long irValue = particleSensor.getIR();
  if (buttonCharacteristic) {
    if (checkForBeat(irValue) == true)
  {
    buttonCharacteristic.writeValue("1");
    Serial.println("blink");
  } else {
    buttonCharacteristic.writeValue("0");
  }
    }
     

    
  // end of while peripheral connected
  // turn the LED off for good measure:
  digitalWrite(LED_BUILTIN, LOW);
  Serial.println("Peripheral disconnected");
}
