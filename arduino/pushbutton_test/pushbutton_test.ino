const int BUTTON = 9; // Naming switch button pin
const int LED = 3;   // Namin LED pin
int BUTTONstate = 0; // A variable to store Button Status / Input

void setup(){ 
  pinMode(LED, OUTPUT);
  pinMode (BUTTON, INPUT);
 }
 
void loop() {
  BUTTONstate = digitalRead(BUTTON);  // Reading button status / input
  Serial.println(BUTTONstate);
  if (BUTTONstate == HIGH)  // Condition to check button input
    {
      digitalWrite(LED, HIGH);
    }
    else
    {
      digitalWrite(LED, LOW);
    }
}
