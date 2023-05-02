  
const int greenPin = 3;       // pin that the LED is attached to
const int pinNumber = 4;
int analogValue = 0;        // value read from the pot
int brightness = 0;  
int frequency = 0; 

void setup() {
  // initialize serial communications at 9600 bps:
    Serial.begin(9600);
    // declare the led pin as an output:
    pinMode(greenPin, OUTPUT);
    pinMode(pinNumber, OUTPUT);
}
 
void loop() {
   analogValue = analogRead(A1);    // read the pot value
   Serial.println(analogValue);
    brightness = analogValue /10;       //divide by 4 to fit in a byte
    analogWrite(greenPin, brightness);   // PWM the LED with the brightness value
    frequency = (analogValue /2) * 10;
    Serial.println(frequency);
    analogWrite(pinNumber, brightness);
    //tone(pinNumber, frequency);
    Serial.println(brightness);
 }
