/* HueBlink example for ArduinoHttpClient library
   Uses ArduinoHttpClient library to control Philips Hue
   For more on Hue developer API see http://developer.meethue.com
   For more on the ArduinoHttpClient, install the library from the
   Library Manager.
  To control a light, the Hue expects a HTTP PUT request to:
  http://hue.hub.address/api/hueUserName/lights/lightNumber/state
  The body of the PUT request looks like this:
  {"on": true} or {"on":false}
  This example  shows how to concatenate Strings to assemble the
  PUT request and the body of the request.
  note: WiFi SSID and password are stored in arduino_secrets.h file.
  If it is not present, add a new tab, call it "arduino_secrets.h" 
  and add the following defines, and change to your own values:
  #define SECRET_SSID "ssid"    
  #define SECRET_PASS "password"
  
   modified 1 Mar 2022 
   by Tom Igoe (tigoe)
*/

#include <SPI.h>
//#include <WiFi101.h>  // for MKR1000
#include <WiFiNINA.h>   // for Nano 33 IoT, MKR1010
#include <ArduinoHttpClient.h>
#include "arduino_secrets.h"
#include "MadgwickAHRS.h"
#include "Arduino_LSM6DS3.h"

int status = WL_IDLE_STATUS;      // the Wifi radio's status
char hueHubIP[] = "172.22.151.226";  // IP address of the HUE bridge
String hueUserName = "ERMsEzqOpFH6q4QvXiMamgOxwbMFi7ifekKgY9nj"; // hue bridge username

// make a wifi instance and a HttpClient instance:
WiFiClient wifi;
HttpClient httpClient = HttpClient(wifi, hueHubIP);
// change the values of these two in the arduino_serets.h file:
char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASS;

// initialize a Madgwick filter:
Madgwick filter;
const float sensorRate = 104.00;

void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial); // wait for serial port to connect.
  //imu
  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU");
    // stop here if you can't access the IMU:
    while (true);
  }
  // attempt to connect to Wifi network:
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network:
    status = WiFi.begin(ssid, pass);
  }

  // you're connected now, so print out the data:
  Serial.print("You're connected to the network IP = ");
  IPAddress ip = WiFi.localIP();
  Serial.println(ip);


      // start the filter to run at the sample rate:
  filter.begin(sensorRate);
  //sendRequest(19, "on", "true");
}
int hue = 0;
void loop() {

      // values for acceleration and rotation:
  float xAcc, yAcc, zAcc;
  float xGyro, yGyro, zGyro;
  float roll, pitch, heading;
    // if both accelerometer and gyrometer are ready to be read:

  if (IMU.accelerationAvailable() && IMU.gyroscopeAvailable()) {
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
      }

      Serial.println(String(pitch*100));
     // sendRequest(19, "bri", String(map(heading));
      sendRequest(19, "hue", String(hue));
      hue = hue+ 1200;
      //delay(10);
      
    
  //sendRequest(19, "on", "true");   // turn light on
  //delay(4000);                    // wait 4 seconds
  //sendRequest(19, "on", "false");  // turn light off
  //delay(4000);                    // wait 4 seconds
}

void sendRequest(int light, String cmd, String value) {
  // make a String for the HTTP request path:
  String request = "/api/" + hueUserName;
  request += "/lights/";
  request += light;
  request += "/state/";

  String contentType = "application/json";

  // make a string for the JSON command:
  String hueCmd = "{\"" + cmd;
  hueCmd += "\":";
  hueCmd += value;
  hueCmd += ",\"sat\":254,\"bri\":254}";
  // see what you assembled to send:
  Serial.print("PUT request to server: ");
  Serial.println(request);
  Serial.print("JSON command to server: ");

  // make the PUT request to the hub:
  httpClient.put(request, contentType, hueCmd);
  
  // read the status code and body of the response
  int statusCode = httpClient.responseStatusCode();
  String response = httpClient.responseBody();

  Serial.println(hueCmd);
  Serial.print("Status code from server: ");
  Serial.println(statusCode);
  Serial.print("Server response: ");
  Serial.println(response);
  Serial.println();
}
