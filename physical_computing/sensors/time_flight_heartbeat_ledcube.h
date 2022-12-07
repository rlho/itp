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
  while (!Serial)
    ;
  // configure the button pin as input:
  pinMode(buttonPin, INPUT_PULLUP);
  // configure the built-in LED as an output:
  pinMode(LED_BUILTIN, OUTPUT);

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

  if (sensor0.isRangeComplete())
  {
    int result0 = sensor0.readRangeResult();
    Serial.print("result0: ");
    Serial.println(result0);
  }
  long irValue = particleSensor.getIR();

  if (checkForBeat(irValue) == true)
  {
    turnEverythingOn();
    Serial.println("blink");
  }
  else
  {
  }
}

////////////////////////////////////////////////////////////turn all on
void turnEverythingOn()
{
  for (int i = 0; i < 16; i++)
  {
    digitalWrite(column[i], 0);
  }
  // turning on layers
  for (int i = 0; i < 4; i++)
  {
    digitalWrite(layer[i], 1);
  }
}

///////////////////////////////////////////////////////turn columns off
void turnColumnsOff()
{
  for (int i = 0; i < 16; i++)
  {
    digitalWrite(column[i], 1);
  }
}

// turn all off

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

/////////////////////////////////////////////////////////////flicker on
void flickerOn()
{
  int i = 150;
  while (i != 0)
  {
    turnEverythingOn();
    delay(i);
    turnEverythingOff();
    delay(i);
    i -= 5;
  }
}
//////////////turn everything on and off by layer up and down NOT TIMED
void turnOnAndOffAllByLayerUpAndDownNotTimed()
{
  int x = 75;
  for (int i = 5; i != 0; i--)
  {
    turnEverythingOn();
    for (int i = 4; i != 0; i--)
    {
      digitalWrite(layer[i - 1], 0);
      delay(x);
    }
    for (int i = 0; i < 4; i++)
    {
      digitalWrite(layer[i], 1);
      delay(x);
    }
    for (int i = 0; i < 4; i++)
    {
      digitalWrite(layer[i], 0);
      delay(x);
    }
    for (int i = 4; i != 0; i--)
    {
      digitalWrite(layer[i - 1], 1);
      delay(x);
    }
  }
}
//////////////////////////turn everything on and off by column sideways
void turnOnAndOffAllByColumnSideways()
{
  int x = 75;
  turnEverythingOff();
  // turn on layers
  for (int i = 0; i < 4; i++)
  {
    digitalWrite(layer[i], 1);
  }
  for (int y = 0; y < 3; y++)
  {
    // turn on 0-3
    for (int i = 0; i < 4; i++)
    {
      digitalWrite(column[i], 0);
      delay(x);
    }
    // turn on 4-7
    for (int i = 4; i < 8; i++)
    {
      digitalWrite(column[i], 0);
      delay(x);
    }
    // turn on 8-11
    for (int i = 8; i < 12; i++)
    {
      digitalWrite(column[i], 0);
      delay(x);
    }
    // turn on 12-15
    for (int i = 12; i < 16; i++)
    {
      digitalWrite(column[i], 0);
      delay(x);
    }
    // turn off 0-3
    for (int i = 0; i < 4; i++)
    {
      digitalWrite(column[i], 1);
      delay(x);
    }
    // turn off 4-7
    for (int i = 4; i < 8; i++)
    {
      digitalWrite(column[i], 1);
      delay(x);
    }
    // turn off 8-11
    for (int i = 8; i < 12; i++)
    {
      digitalWrite(column[i], 1);
      delay(x);
    }
    // turn off 12-15
    for (int i = 12; i < 16; i++)
    {
      digitalWrite(column[i], 1);
      delay(x);
    }
    // turn on 12-15
    for (int i = 12; i < 16; i++)
    {
      digitalWrite(column[i], 0);
      delay(x);
    }
    // turn on 8-11
    for (int i = 8; i < 12; i++)
    {
      digitalWrite(column[i], 0);
      delay(x);
    }
    // turn on 4-7
    for (int i = 4; i < 8; i++)
    {
      digitalWrite(column[i], 0);
      delay(x);
    }
    // turn on 0-3
    for (int i = 0; i < 4; i++)
    {
      digitalWrite(column[i], 0);
      delay(x);
    }
    // turn off 12-15
    for (int i = 12; i < 16; i++)
    {
      digitalWrite(column[i], 1);
      delay(x);
    }
    // turn off 8-11
    for (int i = 8; i < 12; i++)
    {
      digitalWrite(column[i], 1);
      delay(x);
    }
    // turn off 4-7
    for (int i = 4; i < 8; i++)
    {
      digitalWrite(column[i], 1);
      delay(x);
    }
    // turn off 0-3
    for (int i = 0; i < 4; i++)
    {
      digitalWrite(column[i], 1);
      delay(x);
    }
  }
}
/////////////////////////////////////////up and down single layer stomp
void layerstompUpAndDown()
{
  int x = 75;
  for (int i = 0; i < 4; i++)
  {
    digitalWrite(layer[i], 0);
  }
  for (int y = 0; y < 5; y++)
  {
    for (int count = 0; count < 1; count++)
    {
      for (int i = 0; i < 4; i++)
      {
        digitalWrite(layer[i], 1);
        delay(x);
        digitalWrite(layer[i], 0);
      }
      for (int i = 4; i != 0; i--)
      {
        digitalWrite(layer[i - 1], 1);
        delay(x);
        digitalWrite(layer[i - 1], 0);
      }
    }
    for (int i = 0; i < 4; i++)
    {
      digitalWrite(layer[i], 1);
      delay(x);
    }
    for (int i = 4; i != 0; i--)
    {
      digitalWrite(layer[i - 1], 0);
      delay(x);
    }
  }
}
////////////////////////////////////////////////////////////flicker off
void flickerOff()
{
  turnEverythingOn();
  for (int i = 0; i != 150; i += 5)
  {
    turnEverythingOff();
    delay(i + 50);
    turnEverythingOn();
    delay(i);
  }
}
///////////////////////////////////////////around edge of the cube down
void aroundEdgeDown()
{
  for (int x = 200; x != 0; x -= 50)
  {
    turnEverythingOff();
    for (int i = 4; i != 0; i--)
    {
      digitalWrite(layer[i - 1], 1);
      digitalWrite(column[5], 0);
      digitalWrite(column[6], 0);
      digitalWrite(column[9], 0);
      digitalWrite(column[10], 0);

      digitalWrite(column[0], 0);
      delay(x);
      digitalWrite(column[0], 1);
      digitalWrite(column[4], 0);
      delay(x);
      digitalWrite(column[4], 1);
      digitalWrite(column[8], 0);
      delay(x);
      digitalWrite(column[8], 1);
      digitalWrite(column[12], 0);
      delay(x);
      digitalWrite(column[12], 1);
      digitalWrite(column[13], 0);
      delay(x);
      digitalWrite(column[13], 1);
      digitalWrite(column[15], 0);
      delay(x);
      digitalWrite(column[15], 1);
      digitalWrite(column[14], 0);
      delay(x);
      digitalWrite(column[14], 1);
      digitalWrite(column[11], 0);
      delay(x);
      digitalWrite(column[11], 1);
      digitalWrite(column[7], 0);
      delay(x);
      digitalWrite(column[7], 1);
      digitalWrite(column[3], 0);
      delay(x);
      digitalWrite(column[3], 1);
      digitalWrite(column[2], 0);
      delay(x);
      digitalWrite(column[2], 1);
      digitalWrite(column[1], 0);
      delay(x);
      digitalWrite(column[1], 1);
    }
  }
}
/////////////////////////////////////////////////////////random flicker
void randomflicker()
{
  turnEverythingOff();
  int x = 10;
  for (int i = 0; i != 750; i += 2)
  {
    int randomLayer = random(0, 4);
    int randomColumn = random(0, 16);

    digitalWrite(layer[randomLayer], 1);
    digitalWrite(column[randomColumn], 0);
    delay(x);
    digitalWrite(layer[randomLayer], 0);
    digitalWrite(column[randomColumn], 1);
    delay(x);
  }
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
/////////////////////////////////////////////////////diagonal rectangle
void diagonalRectangle()
{
  int x = 350;
  turnEverythingOff();
  for (int count = 0; count < 5; count++)
  {
    // top left
    for (int i = 0; i < 8; i++)
    {
      digitalWrite(column[i], 0);
    }
    digitalWrite(layer[3], 1);
    digitalWrite(layer[2], 1);
    delay(x);
    turnEverythingOff();
    // middle middle
    for (int i = 4; i < 12; i++)
    {
      digitalWrite(column[i], 0);
    }
    digitalWrite(layer[1], 1);
    digitalWrite(layer[2], 1);
    delay(x);
    turnEverythingOff();
    // bottom right
    for (int i = 8; i < 16; i++)
    {
      digitalWrite(column[i], 0);
    }
    digitalWrite(layer[0], 1);
    digitalWrite(layer[1], 1);
    delay(x);
    turnEverythingOff();
    // bottom middle
    for (int i = 4; i < 12; i++)
    {
      digitalWrite(column[i], 0);
    }
    digitalWrite(layer[0], 1);
    digitalWrite(layer[1], 1);
    delay(x);
    turnEverythingOff();
    // bottom left
    for (int i = 0; i < 8; i++)
    {
      digitalWrite(column[i], 0);
    }
    digitalWrite(layer[0], 1);
    digitalWrite(layer[1], 1);
    delay(x);
    turnEverythingOff();
    // middle middle
    for (int i = 4; i < 12; i++)
    {
      digitalWrite(column[i], 0);
    }
    digitalWrite(layer[1], 1);
    digitalWrite(layer[2], 1);
    delay(x);
    turnEverythingOff();
    // top right
    for (int i = 8; i < 16; i++)
    {
      digitalWrite(column[i], 0);
    }
    digitalWrite(layer[2], 1);
    digitalWrite(layer[3], 1);
    delay(x);
    turnEverythingOff();
    // top middle
    for (int i = 4; i < 12; i++)
    {
      digitalWrite(column[i], 0);
    }
    digitalWrite(layer[2], 1);
    digitalWrite(layer[3], 1);
    delay(x);
    turnEverythingOff();
  }
  // top left
  for (int i = 0; i < 8; i++)
  {
    digitalWrite(column[i], 0);
  }
  digitalWrite(layer[3], 1);
  digitalWrite(layer[2], 1);
  delay(x);
  turnEverythingOff();
}
//////////////////////////////////////////////////////////////propeller
void propeller()
{
  turnEverythingOff();
  int x = 90;
  for (int y = 4; y > 0; y--)
  {
    for (int i = 0; i < 6; i++)
    {
      // turn on layer
      digitalWrite(layer[y - 1], 1);
      // a1
      turnColumnsOff();
      digitalWrite(column[0], 0);
      digitalWrite(column[5], 0);
      digitalWrite(column[10], 0);
      digitalWrite(column[15], 0);
      delay(x);
      // b1
      turnColumnsOff();
      digitalWrite(column[4], 0);
      digitalWrite(column[5], 0);
      digitalWrite(column[10], 0);
      digitalWrite(column[11], 0);
      delay(x);
      // c1
      turnColumnsOff();
      digitalWrite(column[6], 0);
      digitalWrite(column[7], 0);
      digitalWrite(column[8], 0);
      digitalWrite(column[9], 0);
      delay(x);
      // d1
      turnColumnsOff();
      digitalWrite(column[3], 0);
      digitalWrite(column[6], 0);
      digitalWrite(column[9], 0);
      digitalWrite(column[12], 0);
      delay(x);
      // d2
      turnColumnsOff();
      digitalWrite(column[2], 0);
      digitalWrite(column[6], 0);
      digitalWrite(column[9], 0);
      digitalWrite(column[13], 0);
      delay(x);
      // d3
      turnColumnsOff();
      digitalWrite(column[1], 0);
      digitalWrite(column[5], 0);
      digitalWrite(column[10], 0);
      digitalWrite(column[14], 0);
      delay(x);
    }
  }
  // d4
  turnColumnsOff();
  digitalWrite(column[0], 0);
  digitalWrite(column[5], 0);
  digitalWrite(column[10], 0);
  digitalWrite(column[15], 0);
  delay(x);
}
//////////////////////////////////////////////////////spiral in and out
void spiralInAndOut()
{
  turnEverythingOn();
  int x = 60;
  for (int i = 0; i < 6; i++)
  {
    // spiral in clockwise
    digitalWrite(column[0], 1);
    delay(x);
    digitalWrite(column[1], 1);
    delay(x);
    digitalWrite(column[2], 1);
    delay(x);
    digitalWrite(column[3], 1);
    delay(x);
    digitalWrite(column[7], 1);
    delay(x);
    digitalWrite(column[11], 1);
    delay(x);
    digitalWrite(column[15], 1);
    delay(x);
    digitalWrite(column[14], 1);
    delay(x);
    digitalWrite(column[13], 1);
    delay(x);
    digitalWrite(column[12], 1);
    delay(x);
    digitalWrite(column[8], 1);
    delay(x);
    digitalWrite(column[4], 1);
    delay(x);
    digitalWrite(column[5], 1);
    delay(x);
    digitalWrite(column[6], 1);
    delay(x);
    digitalWrite(column[10], 1);
    delay(x);
    digitalWrite(column[9], 1);
    delay(x);
    ///////////////////////////////////////spiral out counter clockwise
    digitalWrite(column[9], 0);
    delay(x);
    digitalWrite(column[10], 0);
    delay(x);
    digitalWrite(column[6], 0);
    delay(x);
    digitalWrite(column[5], 0);
    delay(x);
    digitalWrite(column[4], 0);
    delay(x);
    digitalWrite(column[8], 0);
    delay(x);
    digitalWrite(column[12], 0);
    delay(x);
    digitalWrite(column[13], 0);
    delay(x);
    digitalWrite(column[14], 0);
    delay(x);
    digitalWrite(column[15], 0);
    delay(x);
    digitalWrite(column[11], 0);
    delay(x);
    digitalWrite(column[7], 0);
    delay(x);
    digitalWrite(column[3], 0);
    delay(x);
    digitalWrite(column[2], 0);
    delay(x);
    digitalWrite(column[1], 0);
    delay(x);
    digitalWrite(column[0], 0);
    delay(x);
    ///////////////////////////////////////spiral in counter clock wise
    digitalWrite(column[0], 1);
    delay(x);
    digitalWrite(column[4], 1);
    delay(x);
    digitalWrite(column[8], 1);
    delay(x);
    digitalWrite(column[12], 1);
    delay(x);
    digitalWrite(column[13], 1);
    delay(x);
    digitalWrite(column[14], 1);
    delay(x);
    digitalWrite(column[15], 1);
    delay(x);
    digitalWrite(column[11], 1);
    delay(x);
    digitalWrite(column[7], 1);
    delay(x);
    digitalWrite(column[3], 1);
    delay(x);
    digitalWrite(column[2], 1);
    delay(x);
    digitalWrite(column[1], 1);
    delay(x);
    digitalWrite(column[5], 1);
    delay(x);
    digitalWrite(column[9], 1);
    delay(x);
    digitalWrite(column[10], 1);
    delay(x);
    digitalWrite(column[6], 1);
    delay(x);
    //////////////////////////////////////////////spiral out clock wise
    digitalWrite(column[6], 0);
    delay(x);
    digitalWrite(column[10], 0);
    delay(x);
    digitalWrite(column[9], 0);
    delay(x);
    digitalWrite(column[5], 0);
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
    digitalWrite(column[0], 0);
    delay(x);
  }
}
//////////////////////////////////////go through all leds one at a time
void goThroughAllLedsOneAtATime()
{
  int x = 15;
  turnEverythingOff();
  for (int y = 0; y < 5; y++)
  {
    // 0-3
    for (int count = 4; count != 0; count--)
    {
      digitalWrite(layer[count - 1], 1);
      for (int i = 0; i < 4; i++)
      {
        digitalWrite(column[i], 0);
        delay(x);
        digitalWrite(column[i], 1);
        delay(x);
      }
      digitalWrite(layer[count - 1], 0);
    }
    // 4-7
    for (int count = 0; count < 4; count++)
    {
      digitalWrite(layer[count], 1);
      for (int i = 4; i < 8; i++)
      {
        digitalWrite(column[i], 0);
        delay(x);
        digitalWrite(column[i], 1);
        delay(x);
      }
      digitalWrite(layer[count], 0);
    }
    // 8-11
    for (int count = 4; count != 0; count--)
    {
      digitalWrite(layer[count - 1], 1);
      for (int i = 8; i < 12; i++)
      {
        digitalWrite(column[i], 0);
        delay(x);
        digitalWrite(column[i], 1);
        delay(x);
      }
      digitalWrite(layer[count - 1], 0);
    }
    // 12-15
    for (int count = 0; count < 4; count++)
    {
      digitalWrite(layer[count], 1);
      for (int i = 12; i < 16; i++)
      {
        digitalWrite(column[i], 0);
        delay(x);
        digitalWrite(column[i], 1);
        delay(x);
      }
      digitalWrite(layer[count], 0);
    }
  }
}