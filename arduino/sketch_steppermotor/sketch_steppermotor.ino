const int pwmPin = 7;
const int in1Pin = 11;
const int in2Pin = 10;

void setup() {
  pinMode(pwmPin, OUTPUT);
  pinMode(in1Pin, OUTPUT);
  pinMode(in2Pin, OUTPUT);
}

void loop() {
  digitalWrite(in1Pin, HIGH);
  digitalWrite(in2Pin, LOW);

  // 0-255の範囲でモーターの速度を設定します。最大速度は255です。
  analogWrite(pwmPin, 255);

  // ここでポンプの動作時間を設定できます（ミリ秒単位）
  delay(10000);
}
