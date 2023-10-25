boolean numbers[10][7] = {
        { 1, 1, 1, 1, 1, 1, 0 },  // 0
        { 0, 1, 1, 0, 0, 0, 0 },  // 1
        { 1, 1, 0, 1, 1, 0, 1 },  // 2
        { 1, 1, 1, 1, 0, 0, 1 },  // 3
        { 0, 1, 1, 0, 0, 1, 1 },  // 4
        { 1, 0, 1, 1, 0, 1, 1 },  // 5
        { 1, 0, 1, 1, 1, 1, 1 },  // 6
        { 1, 1, 1, 0, 0, 0, 0 },  // 7
        { 1, 1, 1, 1, 1, 1, 1 },  // 8
        { 1, 1, 1, 1, 0, 1, 1 }   // 9
};
boolean guruguru[6][7]{
        { 1, 0, 0, 0, 0, 0, 0 },
        { 0, 1, 0, 0, 0, 0, 0 },
        { 0, 0, 1, 0, 0, 0, 0 },
        { 0, 0, 0, 1, 0, 0, 0 },
        { 0, 0, 0, 0, 1, 0, 0 },
        { 0, 0, 0, 0, 0, 1, 0 },
};
int pins[7] = {
        9, 10, 11, 12, 8, 7, 13
};
const int seg1Pin = 2;  //2ケタめ
const int seg2Pin = 4;  //1ケタめ
const int seg3Pin = 5;  //ぐるぐる
const int seg4Pin = 6;  //ぐるぐる
const int aPin = 9;
const int bPin = 10;
const int cPin = 11;
const int dPin = 12;
const int ePin = 8;
const int fPin = 7;
const int gPin = 13;
int num1 = 0;
int num2 = 0;
int num3 = 0;
int num4 = 0;
int outsideCount;
bool isguruguru;
int score;

void setup() {
    // put your setup code here, to run once:
    Serial.begin(9600);
    TCCR1A = 0;
    TCCR1B = 0;
    TCNT1 = 3036 + 65536 /3;
    TCCR1B = 0b00000100;
    TIMSK1 |= (1 << TOIE1); //TOIE -> 1
    pinMode(aPin, OUTPUT);
    pinMode(bPin, OUTPUT);
    pinMode(cPin, OUTPUT);
    pinMode(dPin, OUTPUT);
    pinMode(ePin, OUTPUT);
    pinMode(fPin, OUTPUT);
    pinMode(gPin, OUTPUT);
    pinMode(seg1Pin, OUTPUT);
    pinMode(seg2Pin, OUTPUT);
    pinMode(seg3Pin, OUTPUT);
    pinMode(seg4Pin, OUTPUT);
    pinMode(1, OUTPUT);
    isguruguru = false;
    outsideCount = 0;
    digitalWrite(aPin, HIGH);
    digitalWrite(bPin, HIGH);
    digitalWrite(cPin, HIGH);
    digitalWrite(dPin, HIGH);
    digitalWrite(ePin, HIGH);
    digitalWrite(fPin, HIGH);
    digitalWrite(gPin, HIGH);
    score=0;
}

ISR(TIMER1_OVF_vect) {
        outsideCount++;
        TCNT1 = 3036 + 65536 / 2;
}

void loop() {
    int mil = millis();
    while (Serial.available()) {
        char digit = Serial.read();
        if (digit == '\n') continue;
        //score += digit - '0';
        score = score * 10;
        score += digit - '0';
    }
    if (score >= 100 || score == 0) {
        isguruguru = false;
        num4 = score % 10;
        num3 = score / 10 % 10;
        num2 = score / 100 % 10;
        num1 = score / 1000 % 10;
        digitalWrite(seg1Pin, HIGH);
        digitalWrite(seg2Pin, LOW);
        digitalWrite(seg3Pin, LOW);
        digitalWrite(seg4Pin, LOW);
        numPrint(num1);
        delay(5);
        digitalWrite(seg1Pin, LOW);
        digitalWrite(seg2Pin, HIGH);
        digitalWrite(seg3Pin, LOW);
        digitalWrite(seg4Pin, LOW);
        numPrint(num2);
        delay(5);
        digitalWrite(seg1Pin, LOW);
        digitalWrite(seg2Pin, LOW);
        digitalWrite(seg3Pin, HIGH);
        digitalWrite(seg4Pin, LOW);
        numPrint(num3);
        delay(5);
        digitalWrite(seg1Pin, LOW);
        digitalWrite(seg2Pin, LOW);
        digitalWrite(seg3Pin, LOW);
        digitalWrite(seg4Pin, HIGH);
        numPrint(num4);
        delay(5);
    }
    else {
        isguruguru = true;
        num2 = score % 10;
        num1 = score / 10 % 10;
        digitalWrite(seg1Pin, HIGH);
        digitalWrite(seg2Pin, LOW);
        digitalWrite(seg3Pin, LOW);
        digitalWrite(seg4Pin, LOW);
        numPrint(num1);
        delay(5);
        digitalWrite(seg1Pin, LOW);
        digitalWrite(seg2Pin, HIGH);
        digitalWrite(seg3Pin, LOW);
        digitalWrite(seg4Pin, LOW);
        numPrint(num2);
        delay(5);

        digitalWrite(seg1Pin, LOW);
        digitalWrite(seg2Pin, LOW);
        digitalWrite(seg3Pin, HIGH);
        digitalWrite(seg4Pin, LOW);
        guruguruprint(outsideCount%6);
        delay(5);
        digitalWrite(seg1Pin, LOW);
        digitalWrite(seg2Pin, LOW);
        digitalWrite(seg3Pin, LOW);
        digitalWrite(seg4Pin, HIGH);
        guruguruprint(outsideCount%6);
        delay(5);
    }
}

void guruguruprint(int num) {

    for (int i = 0; i <= 6; i++) {
        digitalWrite(pins[i], guruguru[num][i] ? LOW : HIGH);
    }
}

void numPrint(int num) {
    for (int i = 0; i <= 6; i++) {
        digitalWrite(pins[i], numbers[num][i] ? LOW : HIGH);
    }
}
