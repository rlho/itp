#include <Adafruit_NeoPixel.h>
#include "Arduino_LSM6DS3.h"
#include "MadgwickAHRS.h"



// ガンマテーブル
const int PROGMEM gamma8[] = {
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2,
  2, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 5, 5, 5,
  5, 6, 6, 6, 6, 7, 7, 7, 7, 8, 8, 8, 9, 9, 9, 10,
  10, 10, 11, 11, 11, 12, 12, 13, 13, 13, 14, 14, 15, 15, 16, 16,
  17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 24, 24, 25,
  25, 26, 27, 27, 28, 29, 29, 30, 31, 32, 32, 33, 34, 35, 35, 36,
  37, 38, 39, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 50,
  51, 52, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 66, 67, 68,
  69, 70, 72, 73, 74, 75, 77, 78, 79, 81, 82, 83, 85, 86, 87, 89,
  90, 92, 93, 95, 96, 98, 99, 101, 102, 104, 105, 107, 109, 110, 112, 114,
  115, 117, 119, 120, 122, 124, 126, 127, 129, 131, 133, 135, 137, 138, 140, 142,
  144, 146, 148, 150, 152, 154, 156, 158, 160, 162, 164, 167, 169, 171, 173, 175,
  177, 180, 182, 184, 186, 189, 191, 193, 196, 198, 200, 203, 205, 208, 210, 213,
  215, 218, 220, 223, 225, 228, 231, 233, 236, 239, 241, 244, 247, 249, 252, 255
};

float value = 0.1;

const int max_value = 255;
const int dimming_range = 90; // magic number

const float threshold = 0.065; // magic number

// initialize a Madgwick filter:
Madgwick filter;
const float sensorRate = 104.00;

const int neoPixelPin = 5;  // control pin
const int pixelCount = 7;   // number of pixels
int change = 100;             // increment to change hue by

// set up strip:
Adafruit_NeoPixel strip = Adafruit_NeoPixel(pixelCount, neoPixelPin, NEO_GRBW + NEO_KHZ800);

int h = 1000;  // hue, 0-65535
int s = 255;  // saturation 0-255
int i = 255;  // intensity 0-255

void setup() {
   Serial.begin(9600);
    if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU");
    // stop here if you can't access the IMU:
    while (true);
  }
    // start the filter to run at the sample rate:
  filter.begin(sensorRate);
  
  strip.begin();  // initialize pixel strip
  strip.clear();  // turn all LEDs off
  strip.show();   // update strip
}

void loop() {

      // values for acceleration and rotation:
  float xAcc, yAcc, zAcc;
  float xGyro, yGyro, zGyro;
  float roll, pitch, heading;
    // if both accelerometer and gyrometer are ready to be read:
  if (IMU.accelerationAvailable() &&
      IMU.gyroscopeAvailable()) {
    // read accelerometer and gyrometer:
    IMU.readAcceleration(xAcc, yAcc, zAcc);

    // print the results:
    IMU.readGyroscope(xGyro, yGyro, zGyro);

    // update the filter, which computes orientation:
    filter.updateIMU(xGyro, yGyro, zGyro, xAcc, yAcc, zAcc);

    // print the heading, pitch and roll
    roll = filter.getRoll();
    pitch = filter.getPitch();
    heading = filter.getYaw();
    Serial.print("Orientation: ");
    Serial.print(heading);
    Serial.print(" ");
    Serial.print(pitch);
    Serial.print(" ");
    Serial.println(roll);

    if(abs(roll) > 6) {
    shaking_moving();
    } else {
      normal_moving();
    }
  }


  

}

void normal_moving() {

    // create a single color from hue, sat, intensity:
  long color = strip.ColorHSV(h, s, i);


 if (value < 0.5) {
    value = value + 2.0 * value * value;
  } else if (value >= 0.5) {
    value = value - 2.0 * (1.0 - value) * (1.0 - value);
  }

  if (value <= (0.0 + threshold) || (1.0 - threshold) <= value) {
    value = random(0.0 + (threshold * 1000),1000 - (threshold * 1000)) / 1000.0;
  }

  int value_1 = max_value - dimming_range + (value * dimming_range);
  int value_2 = max_value - (value * dimming_range);

  //analogWrite(led_pin_1,  pgm_read_byte(&(gamma8[value_1])));
  //analogWrite(led_pin_2,  pgm_read_byte(&(gamma8[value_2])));


  // loop over all the pixels:
    int randNumber = random(7);
    strip.setPixelColor(randNumber, color);
    strip.setBrightness(pgm_read_byte(&(gamma8[value_1])));
    strip.show();  // update the strip
    delay(100);
  

  // increment hue to fade from red (0) to reddish orange (2400) and back:
  h = h + change;
  // when it hits the boundaries, reverse the direction of change:
  if (h <= 0 || h >= 2400) {
    change = -change;
  }
  // constrin hue to 0-2400:
  h = constrain(h, 0, 2400);
}


void shaking_moving() {
    // create a single color from hue, sat, intensity:
  long color = strip.ColorHSV(h, s, i);

    if (value < 0.5) {
    value = value + 2.0 * value * value;
  } else if (value >= 0.5) {
    value = value - 2.0 * (1.0 - value) * (1.0 - value);
  }

  int max_value = value * 255;
  
  // loop over all the pixels:
    int randNumber = random(7);
    strip.setPixelColor(randNumber, color);
    strip.setBrightness(pgm_read_byte(&(gamma8[max_value])));
    strip.show();  // update the strip
    delay(100);
  

  // increment hue to fade from red (0) to reddish orange (2400) and back:
  h = h + change;
  // when it hits the boundaries, reverse the direction of change:
  if (h <= 0 || h >= 2400) {
    change = -change;
  }
  // constrin hue to 0-2400:
  h = constrain(h, 0, 2400);
  
}
