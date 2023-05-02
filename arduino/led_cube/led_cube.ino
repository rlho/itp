// 階層ごとのカソード
// ↑ 上側
// 3
// 2
// 1
// ↓ 基盤側
int fl1 = 4; 
int fl2 = 3; 
int fl3 = 2; 

// セグメントごとのアノード
// ポジション
// ABC → ワイヤー側
// DEF → ワイヤー側
// GHI → ワイヤー側
int segA = 5; 
int segB = 6; 
int segC = 7;
int segD = 8;
int segE = 9;
int segF = 10;
int segG = 11;
int segH = 12;
int segI = 13;

int dinamicDelay = 1; // ダイナミック点灯時のディレイ 6回実行される (1の場合6ms)
int loopDinamicCount = 10; // ループで実行するダイナミック点灯の回数 (100だとx6, 600msごとの遷移)

// パターンは 
// フロア1 ABCDEFGHI, フロア2 ABCDEFGHI, フロア3 ABCDEFGHI
// の順に1か0で表現、1がオン、0がオフ
// 配列の長さが25までのようなのでパターンの限界は25

// テスト
//int patternLength = 1;
//String patterns[] = {
//  "100000000000000000000000000"
//};

// 下から上昇
//int patternLength = 3;
//String patterns[] = {
//  "111111111000000000000000000",
//  "000000000111111111000000000",
//  "000000000000000000111111111"
//};

// うずまき
int patternLength = 26;
String patterns[] = {
  "110000000000000000000000000",
  "011000000000000000000000000",
  "001001000000000000000000000",
  "000001001000000000000000000",
  "000000011000000000000000000",
  "000000110000000000000000000",
  "000100100000000000000000000",
  "000110000000000000000000000",
  "000010000000010000000000000",
  "000000000000011000000000000",
  "000000000000001001000000000",
  "000000000000000011000000000",
  "000000000000000110000000000",
  "000000000000100100000000000",
  "000000000100100000000000000",
  "000000000110000000000000000",
  "000000000011000000000000000",
  "000000000001000000001000000",
  "000000000000000000001001000",
  "000000000000000000000001001",
  "000000000000000000000000011",
  "000000000000000000000000110",
  "000000000000000000000100100",
  "000000000000000000100100000",
  "000000000000000000110000000",
};

void setup() {
  pinMode(fl1, OUTPUT);
  pinMode(fl2, OUTPUT);
  pinMode(fl3, OUTPUT);

  pinMode(segA, OUTPUT);
  pinMode(segB, OUTPUT);
  pinMode(segC, OUTPUT);
  pinMode(segD, OUTPUT);
  pinMode(segE, OUTPUT);
  pinMode(segF, OUTPUT);
  pinMode(segG, OUTPUT);
  pinMode(segH, OUTPUT);
  pinMode(segI, OUTPUT);

  Serial.begin(9600);
}

void clearDisplay() {
  digitalWrite(fl1, HIGH);
  digitalWrite(fl2, HIGH);
  digitalWrite(fl3, HIGH);

  digitalWrite(segA, LOW);
  digitalWrite(segB, LOW);
  digitalWrite(segC, LOW);
  digitalWrite(segD, LOW);
  digitalWrite(segE, LOW);
  digitalWrite(segF, LOW);
  digitalWrite(segG, LOW);
  digitalWrite(segH, LOW);
  digitalWrite(segI, LOW);

  delay(dinamicDelay);
}

bool onoff(String pattern, int index) {
  char c = pattern.charAt(index);
  return c == '1';
}

void showPattern(String pattern) {
  int i = 0;
  clearDisplay();
  digitalWrite(fl1, LOW);
  digitalWrite(segA, onoff(pattern, i++));
  digitalWrite(segB, onoff(pattern, i++));
  digitalWrite(segC, onoff(pattern, i++));
  digitalWrite(segD, onoff(pattern, i++));
  digitalWrite(segE, onoff(pattern, i++));
  digitalWrite(segF, onoff(pattern, i++));
  digitalWrite(segG, onoff(pattern, i++));
  digitalWrite(segH, onoff(pattern, i++));
  digitalWrite(segI, onoff(pattern, i++));
  delay(dinamicDelay);

  clearDisplay();
  digitalWrite(fl2, LOW);
  digitalWrite(segA, onoff(pattern, i++));
  digitalWrite(segB, onoff(pattern, i++));
  digitalWrite(segC, onoff(pattern, i++));
  digitalWrite(segD, onoff(pattern, i++));
  digitalWrite(segE, onoff(pattern, i++));
  digitalWrite(segF, onoff(pattern, i++));
  digitalWrite(segG, onoff(pattern, i++));
  digitalWrite(segH, onoff(pattern, i++));
  digitalWrite(segI, onoff(pattern, i++));
  delay(dinamicDelay);

  clearDisplay();
  digitalWrite(fl3, LOW);
  digitalWrite(segA, onoff(pattern, i++));
  digitalWrite(segB, onoff(pattern, i++));
  digitalWrite(segC, onoff(pattern, i++));
  digitalWrite(segD, onoff(pattern, i++));
  digitalWrite(segE, onoff(pattern, i++));
  digitalWrite(segF, onoff(pattern, i++));
  digitalWrite(segG, onoff(pattern, i++));
  digitalWrite(segH, onoff(pattern, i++));
  digitalWrite(segI, onoff(pattern, i++));
  delay(dinamicDelay);
}

void loop() {
  for (int i = 0; i < patternLength; i++) {
    for (int j = 0; j < loopDinamicCount; j++) {
      showPattern(patterns[i]);
    } 
  }
}
