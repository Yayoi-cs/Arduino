#include <LiquidCrystal.h>

//pin Value
//LEDsettings
const int lED1 = A2;   // @*****@
const int lED2 = A3;   // *@***@*
const int lED3 = A4;   // **@*@**
const int lED4 = A5;   // ***@***
//LCD settings
const int rS = 11;
const int rW = 10;
const int e = 9;
const int dB7 = 7;
const int dB6 = 6;
const int dB5 = 5;
const int dB4 = 4;
//Button settings
const int buttonHigh = 13;
const int buttonLow = 12;
//
const int isSounted = 3;

// Timer counter
int timerCount = 0;
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
    digitalWrite(lED1, LOW);
    digitalWrite(lED2, LOW);
    digitalWrite(lED3, LOW);
    digitalWrite(lED4, LOW);

    // Timer initilization
    TCCR1A = 0; // 初期化
    TCCR1B = 0; // 初期化
    // 0.5秒ずつに割り込まれるように設定
    TCNT1 = 3036 + 65536 / 2;
    TCCR1B = 0b00000100;
    TIMSK1 |= (1 << TOIE1); //TOIE -> 1 enable overflow
}
void displayNumber(int num) {
    lcd.clear();
    lcd.print(num);
    lcd.setCursor(3, 0);
    lcd.print("->");

    // numを1byte整数に丸め込む
    uint8_t num_u8 = constrain(num, 0, 254);
    // シリアル通信で送信
    Serial.write(num_u8);
}

void (*resetFunc)(void) = 0;

ISR(TIMER1_OVF_vect) {
        // タイマの割り込み処理
        switch (timerCount % 4){
            case 0:
                digitalWrite(lED1, HIGH);
                digitalWrite(lED2, LOW);
                digitalWrite(lED3, LOW);
                digitalWrite(lED4, LOW);
                break;
            case 1:
                digitalWrite(lED1, LOW);
                digitalWrite(lED2, HIGH);
                digitalWrite(lED3, LOW);
                digitalWrite(lED4, LOW);
                break;
            case 2:
                digitalWrite(lED1, LOW);
                digitalWrite(lED2, LOW);
                digitalWrite(lED3, HIGH);
                digitalWrite(lED4, LOW);
                break;
            case 3:
                digitalWrite(lED1, LOW);
                digitalWrite(lED2, LOW);
                digitalWrite(lED3, LOW);
                digitalWrite(lED4, HIGH);
        }
        timerCount++;
        Serial.println(millis());

        TCNT1 = 3036 + 65536 / 2;
}

void prologue() {
    lcd.clear();
    lcd.print("Game Start...");
    digitalWrite(lED1, HIGH);
    delay(1000);
    digitalWrite(lED2, HIGH);
    delay(1000);
    digitalWrite(lED3, HIGH);
    delay(1000);
    digitalWrite(lED4, HIGH);
    //delay(1000);
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
    //Serial.println(score);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Game Finish!");
    lcd.setCursor(0, 1);
    lcd.print("check your score");
    String myString = String(score);
    Serial.write(myString.c_str());
    delay(5000);
}
void loop() {
    // put your main code here, to run repeatedly:
    if (digitalRead(buttonHigh) == LOW && digitalRead(buttonLow) == LOW && isStarted == false) {
        prologue();
        perNum = generateRand();
        displayNumber(perNum);
        isStarted = true;
    }
    /*
      if (digitalRead(buttonHigh) == LOW && digitalRead(buttonLow) == LOW && isStarted == true) {
      resetFunc();
      }
    */
    if (isStarted == true) {
        int isHigh = digitalRead(buttonHigh);
        int isLow = digitalRead(buttonLow);
        if (isHigh == 0) {

            lcd.print("high");
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
            lcd.print("low");
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
