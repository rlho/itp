  
const int greenPin = 3;       // pin that the LED is attached to
const int pinNumber = 4;


void setup() {
  // initialize serial communications at 9600 bps:
    Serial.begin(9600);
    // declare the led pin as an output:
    pinMode(greenPin, OUTPUT);
    pinMode(pinNumber, OUTPUT);
}
 
void loop() {
for(int i = 2; i <= 10; i++){
    
    digitalWrite(i, HIGH);
  }
   int analogValue0 = analogRead(A0);
   int analogValue1 = analogRead(A1);
   
    int brightness = analogValue0 /10; 
    int brightness1 = analogValue1/2;
    analogWrite(greenPin, brightness);   // PWM the LED with the brightness value

    analogWrite(pinNumber, brightness1);
    //tone(pinNumber, frequency);
    Serial.println(brightness);
 }
