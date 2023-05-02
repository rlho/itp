/*
  Central duplex communication.

  This example scans for BLE peripherals until one with  the advertised service
  UUID is found. Once discovered and connected, this device
  will monitor the peripheral's button characteristic and use it to set a local LED,
  and will change the peripheral's LED characteristic from a local button.
  The circuit:
  - Arduino MKR WiFi 1010, Arduino Uno WiFi Rev2 board, Arduino Nano 33 IoT,
    Arduino Nano 33 BLE, or Arduino Nano 33 BLE Sense board.
  - Button with pull-down resistor connected to pin 2.
  - LED connected to built-in LED pin.

  You can use it with another board that is compatible with this library
  and running the PeripheralDuplex example.

  created 27 Aug 2019
  by Tom Igoe
  based on Sandeep Mistry's examples
*/

#include <ArduinoBLE.h>
#include <Wire.h>
#include "MAX30105.h"
#include "Adafruit_VL53L0X.h"

#include "heartRate.h"

MAX30105 particleSensor;
int sensor1ShutdownPin = 2;
int sensor2ShutdownPin = 3;

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



// create service
BLEService duplexService("473da924-c93a-11e9-a32f-2a2ae2dbcce4");
// create button characteristic and allow remote device to get notifications
BLEByteCharacteristic buttonCharacteristic("473dab7c-c93a-11e9-a32f-2a2ae2dbcce4",
    BLERead | BLENotify);
// create LED characteristic and allow remote device to read and write
BLEByteCharacteristic ledCharacteristic("473dacc6-c93a-11e9-a32f-2a2ae2dbcce4",
                                        BLERead | BLEWrite);

// peripheral characteristic flags:
bool ledAvailable = false;
bool buttonSubscribed = false;

void setup() {

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


  // Bluetooth
  if (!BLE.begin()) {
    Serial.println("starting Bluetooth® Low Energy module failed!");
    while (1);
  }
  // set the local name peripheral advertises
  BLE.setDeviceName("TimeFlightHeartBeat");
  BLE.setAdvertisedService(duplexService);
  // add the characteristics to the service
  duplexService.addCharacteristic(ledCharacteristic);
  duplexService.addCharacteristic(buttonCharacteristic);

   // start advertising
  BLE.advertise();
  Serial.println("Bluetooth® device active, waiting for connections...");

}

void loop() {
// poll for Bluetooth® Low Energy events
   BLEDevice central = BLE.central();
     if (central) {
    Serial.println("Got a central");
    while (central.connected()) {
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
  }

     while (buttonCharacteristic.canRead()) { //true if characteristic is readable
    buttonCharacteristic.read(); //
    if (buttonCharacteristic.valueLength() > 0) { //the size of the value in bytes
      unsigned int dataHex[42] = {};
      buttonCharacteristic.readValue(dataHex, 42);
      //unsigned int dataHex[14] = {};
      unsigned char charDataHex[14] = {};
      String s = "";
      for (int i = 0; i < 42; i++) {
        if(dataHex[i] > 0) {
          Serial.println("pulse");
        Serial.println(dataHex[i]);
 
        }
      }
    }
  }


  }
     }
 



    // when the central disconnects, print it out:
    Serial.print("Disconnected from central: ");
    Serial.println(central.address());
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
