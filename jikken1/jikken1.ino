#include <LiquidCrystal.h>

//pin Value
//LEDsettings
const int lED1 = 2;   // @***@
const int lED2 = 13;  // *@*@*
const int lED3 = 3;   // **@**
//LED settings
const int rS = 10;
const int rW = 11;
const int e = 12;
const int dB7 = 6;
const int dB6 = 7;
const int dB5 = 8;
const int dB4 = 9;
//Button settings
const int buttonHigh = 4;
const int buttonLow = 5;
//
const int isSounted = 3;
//
bool isStarted;
int score;
int perNum;
//LCDInstance
LiquidCrystal lcd(rS, rW, e, dB4, dB5, dB6, dB7);
void setup() {
  //serialSetup
  Serial.begin(9600);
  //setupPins
  pinMode(lED1, OUTPUT);
  pinMode(lED2, OUTPUT);
  pinMode(lED3, OUTPUT);
  pinMode(rS, OUTPUT);
  pinMode(rW, OUTPUT);
  pinMode(e, OUTPUT);
  pinMode(dB4, OUTPUT);
  pinMode(dB5, OUTPUT);
  pinMode(dB6, OUTPUT);
  pinMode(dB7, OUTPUT);
  pinMode(buttonHigh, INPUT);
  pinMode(buttonLow, INPUT);
  pinMode(isSounted, INPUT);
  // put your setup code here, to run once:
  lcd.begin(16, 2);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("High and Low");
  score = 0;
  isStarted = false;
  digitalWrite(lED1, HIGH);
  digitalWrite(lED2, HIGH);
  digitalWrite(lED3, HIGH);
}
void displayNumber(int num) {
  lcd.clear();
  lcd.print(num);
  lcd.setCursor(3, 0);
  lcd.print("->");
}

void (*resetFunc)(void) = 0;

void prologue() {
  lcd.clear();
  lcd.print("Game Start...");
  digitalWrite(lED1, LOW);
  delay(1000);
  digitalWrite(lED2, LOW);
  digitalWrite(lED1, HIGH);
  delay(1000);
  digitalWrite(lED2, HIGH);
  lcd.clear();
}

int generateRand() {
  unsigned long seed = millis();
  randomSeed(seed);
  return random(1, 13);
}

void pointAdder() {
  if (digitalRead(isSounted) == 1) {
    score += 150;
  } else {
    score += 100;
  }
}

void gameFin() {
  Serial.println(score);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("game Finish!");
  lcd.setCursor(0, 1);
  lcd.print("check your score");
  String myString = String(score);
  Serial.write(myString.c_str());
  delay(5000);
}
void loop() {
  // put your main code here, to run repeatedly:
  if (digitalRead(buttonHigh) == LOW && digitalRead(buttonLow) == LOW || isStarted == false) {
    prologue();
    perNum = generateRand();
    displayNumber(perNum);
    isStarted = true;
  }
  if (digitalRead(buttonHigh) == LOW && digitalRead(buttonLow) == LOW && isStarted == true) {
    resetFunc();
  }
  if (isStarted == true) {
    int isHigh = digitalRead(buttonHigh);
    int isLow = digitalRead(buttonLow);
    if (isHigh == 0) {
      int nextNum = generateRand();
      if (nextNum >= perNum) {
        pointAdder();
        lcd.setCursor(1, 1);
        lcd.print("correct");
        delay(1000);
        perNum = nextNum;
        displayNumber(perNum);
        delay(500);
      } else {
        //gameOver
        gameFin();
        isStarted = false;
      }
    }

    if (isLow == 0) {
      int nextNum = generateRand();
      if (nextNum < perNum) {
        pointAdder();
        lcd.setCursor(1, 1);
        lcd.print("correct");
        delay(1000);
        perNum = nextNum;
        displayNumber(perNum);
        delay(500);
      } else {
        //gameOver
        gameFin();
        isStarted = false;
      }
    }
  }
}