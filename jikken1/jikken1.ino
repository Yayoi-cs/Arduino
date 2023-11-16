#include <LiquidCrystal.h>

//pin Value
//LEDsettings
const int lED1 = A5;   // @*****@
const int lED2 = A4;   // *@***@*
const int lED3 = A3;   // **@*@**
const int lED4 = A2;   // ***@***
//LCD setting
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
bool isSounted;

// Timer counter
int timerCount = 0;
//
bool isStarted;
int score;
int perNum;
int perTimerCount;
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
    //lcd.print("High and Low");
    //でばっく
    lcd.print("High and Low");
    score = 0;
    isStarted = false;
    digitalWrite(lED1, LOW);
    digitalWrite(lED2, LOW);
    digitalWrite(lED3, LOW);
    digitalWrite(lED4, LOW);
    isSounted=false;
    perTimerCount=0;
    // Timer initilization
    TCCR1A = 0; // 初期化
    TCCR1B = 0; // 初期化
    // 0.25秒ずつに割り込まれるように設定
    TCNT1 = 3036+((65536-3036)/4*3);
    TCCR1B = 0b00000100;
    TIMSK1 |= (1 << TOIE1); //TOIE -> 1 enable overflow
}
void displayNumber(int num) {
//    lcd.clear();
//    lcd.print(num);
//    lcd.setCursor(3, 0);
//    lcd.print("->");

    String myString = String(num);
    Serial.write(myString.c_str());
}

void (*resetFunc)(void) = 0;

ISR(TIMER1_OVF_vect) {
        if(isStarted==false){
            noTone(3);
            return;
        }
        digitalWrite(lED1,(timerCount%8==0)?HIGH:LOW);
        digitalWrite(lED1,(timerCount%8==2)?HIGH:LOW);
        digitalWrite(lED1,(timerCount%8==4)?HIGH:LOW);
        digitalWrite(lED1,(timerCount%8==6)?HIGH:LOW);
        if((timerCount%8)%2==0&&timerCount%8<=4)
            tone(3,440);
        else if(timerCount%8==6)
            tone(3,480);
        else
            notone(3);
        timerCount++;
        TCNT1 = 3036+((65536-3036)/4*3);
}

void prologue() {
    lcd.clear();
    lcd.print("Game Start...");
    for(int i = 0;i < 4;i++){
        digitalWrite((i==0)?lED1:(i==1)?lED2:(i==2)?lED3:lED4,HIGH);
        delay(1000)
    }
    score = 0;
}

int generateRand() {
    unsigned long seed = millis();
    randomSeed(seed);
    return random(1, 13);
}

void pointAdder() {
    if (isSounted) {
        score+=150;
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
    lcd.print("check your score"); //<= ("check your")
    if(score != 0){ // <= score>0
        score=0;
    }
    String myString = String(score);
    Serial.write(myString.c_str());
    delay(5000);
}
void loop() {
    // put your main code here, to run repeatedly:
    if (digitalRead(buttonHigh) == HIGH && digitalRead(buttonLow) == HIGH && isStarted == false) {
        prologue();
        perNum = generateRand();
        displayNumber(perNum);
        isStarted = true;
    }
    if (digitalRead(buttonHigh) == HIGH && digitalRead(buttonLow) == HIGH && isStarted == true) {
        resetFunc();
    }
    if (isStarted == true) {
        int isHigh = digitalRead(buttonHigh);
        int isLow = digitalRead(buttonLow);
        if (isHigh == 1) {

            lcd.print("high");
            int nextNum = generateRand();
            if (nextNum >= perNum) {
                pointAdder();
                lcd.setCursor(1, 1);
                lcd.print("correct");
                perNum = nextNum;
                displayNumber(perNum);
                delay(500);
            } else {
                //gameOver
                isStarted = false;
                gameFin();
            }
        }

        if (isLow == 1) {
            lcd.print("low");
            int nextNum = generateRand();
            if (nextNum <= perNum) {
                pointAdder();
                lcd.setCursor(1, 1);
                lcd.print("correct");
                perNum = nextNum;
                displayNumber(perNum);
                delay(500);
            } else {
                gameFin();
                isStarted = false;
                gameFin();
            }
        }
    }
}