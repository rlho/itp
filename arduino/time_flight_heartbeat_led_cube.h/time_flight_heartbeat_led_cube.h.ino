// include library
#include "Adafruit_VL53L0X.h"
#include "Servo.h"
#include "MAX30105.h"
#include <ArduinoBLE.h>
#include "heartRate.h"
// Time-flight sensor
Adafruit_VL53L0X sensor0 = Adafruit_VL53L0X();

const int ledPin = LED_BUILTIN; // set ledPin to on-board LED

// Heartbeat
MAX30105 particleSensor;

// Control multiple sensors

int sensor2ShutdownPin = 3;

// LEDs
boolean startLed = false;
const byte RATE_SIZE = 4; // Increase this for more averaging. 4 is good.
byte rates[RATE_SIZE];    // Array of heart rates
byte rateSpot = 0;
long lastBeat = 0; // Time at which the last beat occurred
float beatsPerMinute;
int beatAvg;
// initializing and declaring led rows
int column[16] = {13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0, A7, A6};
// initializing and declaring led layers
int layer[4] = {A3, A2, A1, A0};
int time = 250;
// variables for button:
const int buttonPin = 2;
int oldButtonState = LOW;

void setup()
{

  // Setup LEDs
  // setting rows to ouput
  for (int i = 0; i < 16; i++)
  {
    pinMode(column[i], OUTPUT);
  }
  // setting layers to output
  for (int i = 0; i < 4; i++)
  {
    pinMode(layer[i], OUTPUT);
  }
  // seeding random for random pattern
  randomSeed(analogRead(10));

  Serial.begin(9600);


  // configure the built-in LED as an output:
  pinMode(LED_BUILTIN, OUTPUT);

  // initialize serial, wait 3 seconds for
  // Serial Monitor to open:
  Serial.begin(9600);
  if (!Serial)
    delay(3000);

  // Initialize sensor
  pinMode(ledPin, OUTPUT); // use the LED as an output



  delay(10);

  digitalWrite(sensor2ShutdownPin, HIGH);
  digitalWrite(sensor2ShutdownPin, LOW);


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


  // initialize sensor, stop if it fails:
  if (!sensor0.begin(0x29))
  {
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
  Serial.println("started!");
}

void loop()
{


  long irValue = particleSensor.getIR();
  //Serial.println(irValue);
  if (checkForBeat(irValue) == true)
  {
    Serial.print("get pulse!");
    turnEverythingOff();
    delay(200);
  }
  else {


    if (sensor0.isRangeComplete())
    {
      int result0 = sensor0.readRangeResult();
      Serial.print("result0: ");
      Serial.println(result0);
      if (irValue < 50000 && result0 > 1000) {
        randomRain1();
      }
      else if(result0 > 220){
        turnEverythingOff();
        layer1();
      } else if(result0 > 180) {
        turnEverythingOff();
        layer2();
      }else if(result0 >= 80) {
        turnEverythingOff();
        layer3();
      }
      else {
        turnEverythingOff();
        layer4();
      }
    }
  }
}

////////////////////////////////////////////////////////////turn all on
void blinkLayer1()
{
    digitalWrite(layer[0], 1);


    digitalWrite(column[5], 0);
    digitalWrite(column[6], 0);
    digitalWrite(column[9], 0);
    digitalWrite(column[10], 0);


  for (int i = 0; i < 16; i++)
  {
    digitalWrite(column[i], 0);
  }

}


////////////////////////////////////////////////////////////turn all on
void layer1()
{
    digitalWrite(layer[0], 1);


    digitalWrite(column[5], 0);
    digitalWrite(column[6], 0);
    digitalWrite(column[9], 0);
    digitalWrite(column[10], 0);


  for (int i = 0; i < 16; i++)
  {
    digitalWrite(column[i], 0);
  }

}


////////////////////////////////////////////////////////////turn all on
void layer2()
{

    digitalWrite(layer[0], 1);
    digitalWrite(layer[2], 1);
    
    digitalWrite(column[5], 0);
    digitalWrite(column[6], 0);
    digitalWrite(column[9], 0);
    digitalWrite(column[10], 0);


  for (int i = 0; i < 16; i++)
  {
    digitalWrite(column[i], 0);
  }

}



////////////////////////////////////////////////////////////turn all on
void layer3()
{

    
    digitalWrite(layer[1], 1);
    digitalWrite(layer[2], 1);

    digitalWrite(column[5], 0);
    digitalWrite(column[6], 0);
    digitalWrite(column[9], 0);
    digitalWrite(column[10], 0);
    

    digitalWrite(layer[0], 1);
    for (int i = 0; i < 16; i++)
    {
      digitalWrite(column[i], 0);
    }

}



void layer4()
{

    
    digitalWrite(layer[2], 1);
    digitalWrite(layer[1], 1);

    digitalWrite(column[5], 0);
    digitalWrite(column[6], 0);
    digitalWrite(column[9], 0);
    digitalWrite(column[10], 0);

    digitalWrite(layer[0], 1);
    digitalWrite(layer[3], 1);

    for (int i = 0; i < 16; i++)
    {
      digitalWrite(column[i], 0);
    }

}
////////////////////////////////////////////////////////////turn all on
void turnEverythingOn()
  {
  // turning on layers
  for (int i = 0; i < 4; i++)
  {
    digitalWrite(layer[i], 1);
  }

     int x = 10;

    // spiral in clockwise
    digitalWrite(column[0], 0);
    delay(x);
    digitalWrite(column[1], 0);
    delay(x);
    digitalWrite(column[2], 0);
    delay(x);
    digitalWrite(column[3], 0);
    delay(x);
    digitalWrite(column[7], 0);
    delay(x);
    digitalWrite(column[11], 0);
    delay(x);
    digitalWrite(column[15], 0);
    delay(x);
    digitalWrite(column[14], 0);
    delay(x);
    digitalWrite(column[13], 0);
    delay(x);
    digitalWrite(column[12], 0);
    delay(x);
    digitalWrite(column[8], 0);
    delay(x);
    digitalWrite(column[4], 0);
    delay(x);
    digitalWrite(column[5], 0);
    delay(x);
    digitalWrite(column[6], 0);
    delay(x);
    digitalWrite(column[10], 0);
    delay(x);
    digitalWrite(column[9], 0);
    delay(x);

    

}


void turnEverythingOff()
{
  for (int i = 0; i < 16; i++)
  {
    digitalWrite(column[i], 1);
  }
  for (int i = 0; i < 4; i++)
  {
    digitalWrite(layer[i], 0);
  }
}

////////////////////////////////////////////////////////////random rain
void randomRain1()
{
  turnEverythingOff();
   int randomRow = random(0, 4);
    int randomColumn = random(0, 16);
    digitalWrite(layer[randomRow], 1);
    digitalWrite(column[randomColumn], 0);
}

////////////////////////////////////////////////////////////random rain
void randomRain()
{
  turnEverythingOff();
  int x = 10;
  for (int i = 0; i != 60; i += 2)
  {
    int randomColumn = random(0, 16);
    digitalWrite(column[randomColumn], 0);
    digitalWrite(layer[0], 1);
    delay(x + 50);
    digitalWrite(layer[0], 0);
    digitalWrite(layer[1], 1);
    delay(x);
    digitalWrite(layer[1], 0);
    digitalWrite(layer[2], 1);
    delay(x);
    digitalWrite(layer[2], 0);
    digitalWrite(layer[3], 1);
    delay(x + 50);
    digitalWrite(layer[3], 0);
    digitalWrite(column[randomColumn], 1);
  }
}
