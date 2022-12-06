#include <ArduinoBLE.h>
#include <Wire.h>
#include "MAX30105.h"
#include "Adafruit_VL53L0X.h"
#include "heartRate.h"

MAX30105 particleSensor;

// LEDs
boolean startLed = false;
const byte RATE_SIZE = 4; //Increase this for more averaging. 4 is good.
byte rates[RATE_SIZE]; //Array of heart rates
byte rateSpot = 0;
long lastBeat = 0; //Time at which the last beat occurred
float beatsPerMinute;
int beatAvg;
//initializing and declaring led rows
int column[16]={13,12,11,10,9,8,7,6,5,4,3,2,1,0,A7,A6};
//initializing and declaring led layers
int layer[4]={A3,A2,A1,A0};
int time = 250;
// variables for button:
const int buttonPin = 2;
int oldButtonState = LOW;

//Bluetooth
BLECharacteristic ledCharacteristic;
BLECharacteristic buttonCharacteristic;

// peripheral characteristic flags:
bool ledAvailable = false;
bool buttonSubscribed = false;


void setup() {

  //Setup LEDs
  //setting rows to ouput
  for(int i = 0; i<16; i++)
  {
    pinMode(column[i], OUTPUT);
  }
  //setting layers to output
  for(int i = 0; i<4; i++)
  {
    pinMode(layer[i], OUTPUT);
  }
  //seeding random for random pattern
  randomSeed(analogRead(10));

  Serial.begin(9600);
  while (!Serial);
  // configure the button pin as input:
  pinMode(buttonPin, INPUT_PULLUP);
  // configure the built-in LED as an output:
  pinMode(LED_BUILTIN, OUTPUT);

  //Bluetooth
  // initialize the BLE hardware
  BLE.begin();
  // start scanning for peripherals
  BLE.scanForUuid("473da924-c93a-11e9-a32f-2a2ae2dbcce4");
  Serial.println("central is scanning");
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
  buttonCharacteristic = peripheral.characteristic("473da924-c93a-11e9-a32f-2a2ae2dbcce3");
  ledCharacteristic = peripheral.characteristic("473da924-c93a-11e9-a32f-2a2ae2dbcce4");

  // control cube with distance
    if(ledCharacteristic) {
    while (ledCharacteristic.canRead()) { //true if characteristic is readable
      ledCharacteristic.read(); //
      if (ledCharacteristic.valueLength() > 0) { //the size of the value in bytes
        unsigned int dataHex[42] = {};
        ledCharacteristic.readValue(dataHex, 42);
        //unsigned int dataHex[14] = {};
        unsigned char charDataHex[14] = {};
        String s = "";
        for (int i = 0; i < 42; i++) {
          if(dataHex[i] > 0) {
          Serial.println(dataHex[i]);
          }
        }
      }

      // start cube with pulse
      buttonCharacteristic.read(); //
      if (buttonCharacteristic.valueLength() > 0) { //the size of the value in bytes
        unsigned int dataHex[42] = {};
        buttonCharacteristic.readValue(dataHex, 42);
        //unsigned int dataHex[14] = {};
        unsigned char charDataHex[14] = {};
        String s = "";
        for (int i = 0; i < 42; i++) {
          if(dataHex[i] > 0) {
          startLed = true;
          turnEverythingOn();
          Serial.println(dataHex[i]);
          }
        }
      }
    }
  }
  // end of while peripheral connected
  // turn the LED off for good measure:
  digitalWrite(LED_BUILTIN, LOW);
  Serial.println("Peripheral disconnected");
}



void exploreCharacteristic(BLECharacteristic characteristic) {
  // check if the characteristic is readable
  String thisUuid = String(characteristic.uuid());
  if (characteristic.canSubscribe() &&
      // does it match the UUID you care about?
      thisUuid == "473da924-c93a-11e9-a32f-2a2ae2dbcce4") {
    Serial.println("can subscribe");
    // read the characteristic value
    buttonCharacteristic = characteristic;
    if (buttonCharacteristic.subscribe()) {
      Serial.println("button subscribed");
    }
  }
  // Check if it's writable:
  if (characteristic.canWrite() &&
      // does it match the UUID you care about?
      thisUuid == "473da924-c93a-11e9-a32f-2a2ae2dbcce4") {
    ledCharacteristic = characteristic;
    Serial.println("led available");
  }
}

////////////////////////////////////////////////////////////turn all on
void turnEverythingOn()
{
  for(int i = 0; i<16; i++)
  {
    digitalWrite(column[i], 0);
  }
  //turning on layers
  for(int i = 0; i<4; i++)
  {
    digitalWrite(layer[i], 1);
  }
}

///////////////////////////////////////////////////////turn columns off
void turnColumnsOff()
{
  for(int i = 0; i<16; i++)
  {
    digitalWrite(column[i], 1);
  }
}

//turn all off

void turnEverythingOff()
 {
   for(int i = 0; i<16; i++)
   {
     digitalWrite(column[i], 1);
   }
   for(int i = 0; i<4; i++)
   {
     digitalWrite(layer[i], 0);
   }
 }