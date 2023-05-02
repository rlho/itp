#include "DHT.h"
#define DATA_PIN 4 // D2

DHT dht(DATA_PIN, DHT11);

void setup() {
    Serial.begin(9600);
    dht.begin();
}

void loop() {

 

    float humid = dht.readHumidity();
    float temp = dht.readTemperature();

    if (isnan(humid) || isnan(temp)) {
        Serial.println("Failed...");
        return;
    }
    Serial.print(humid);
     Serial.print(temp);
}
