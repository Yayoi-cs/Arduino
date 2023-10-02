boolean numbers[10][7] = {
  {1,1,1,1,1,1,0},  // 0
  {0,1,1,0,0,0,0},  // 1
  {1,1,0,1,1,0,1},  // 2
  {1,1,1,1,0,0,1},  // 3
  {0,1,1,0,0,1,1},  // 4
  {1,0,1,1,0,1,1},  // 5
  {1,0,1,1,1,1,1},  // 6
  {1,1,1,0,0,0,0},  // 7
  {1,1,1,1,1,1,1},  // 8
  {1,1,1,1,0,1,1}   // 9
  };
boolean guruguru[6][7]{
  {1,0,0,0,0,0,0},
  {0,1,0,0,0,0,0},
  {0,0,1,0,0,0,0},
  {0,0,0,1,0,0,0},
  {0,0,0,0,1,0,0},
  {0,0,0,0,0,1,0},
};
int pins[7] = {
  12,2,3,4,5,6,7
};
const int seg1Pin=11; //2ケタめ
const int seg2Pin=10; //1ケタめ
const int seg3Pin=9; //ぐるぐる
const int seg4Pin=8; //ぐるぐる
const int aPin = 12;
const int bPin = 2;
const int cPin = 3;
const int dPin = 4;
const int ePin = 5;
const int fPin = 6;
const int gPin = 7;
int num1 = 0;
int num2 = 0;
int num3 = 0;
int num4 = 0;
int outsideCount;
bool isguruguru;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(aPin,OUTPUT);
  pinMode(bPin,OUTPUT);
  pinMode(cPin,OUTPUT);
  pinMode(dPin,OUTPUT);
  pinMode(ePin,OUTPUT);
  pinMode(fPin,OUTPUT);
  pinMode(gPin,OUTPUT);
  pinMode(seg1Pin,OUTPUT);
  pinMode(seg2Pin,OUTPUT);
  pinMode(seg3Pin,OUTPUT);
  pinMode(seg4Pin,OUTPUT);
  isguruguru = true;
  outsideCount = 0;
}

void loop() {
  int mil = millis();
  if(mil%300 == 0){
    if(isguruguru==true){
      digitalWrite(seg1Pin,LOW);
      digitalWrite(seg2Pin,LOW);
      digitalWrite(seg3Pin,HIGH);
      digitalWrite(seg4Pin,HIGH);
      guruguruprint(outsideCount % 6);
      outsideCount++;
    }
  }
  if(mil%1000 == 0){
    if(isguruguru==true){
      tone(13,440);
      delay(10);
      noTone(13);
    }
  }
  while(Serial.available()){
    byte signal = (byte)Serial.read();
    int score = signal;
    int counter = signal;
    int keta = 0;
    while(counter != 0){
      counter = counter / 10;
      keta++;
    }
    if(keta<=2){
      num2 = score % 10;
      num1 = score / 10 % 10;
      digitalWrite(seg1Pin,HIGH);
      digitalWrite(seg2Pin,LOW);
      digitalWrite(seg3Pin,LOW);
      digitalWrite(seg4Pin,LOW);
      numPrint(num1);
      digitalWrite(seg1Pin,LOW);
      digitalWrite(seg2Pin,HIGH);
      digitalWrite(seg3Pin,LOW);
      digitalWrite(seg4Pin,LOW);
      numPrint(num2);
      isguruguru = true;
    }
    else {
      isguruguru=false;
      num4=score % 10;
      num3=score /10 % 10;
      num2=score /100 % 10;
      num1=score /1000 % 10;
      digitalWrite(seg1Pin,HIGH);
      digitalWrite(seg2Pin,LOW);
      digitalWrite(seg3Pin,LOW);
      digitalWrite(seg4Pin,LOW);
      numPrint(1);
      digitalWrite(seg1Pin,LOW);
      digitalWrite(seg2Pin,HIGH);
      digitalWrite(seg3Pin,LOW);
      digitalWrite(seg4Pin,LOW);
      numPrint(2);
      digitalWrite(seg1Pin,LOW);
      digitalWrite(seg2Pin,LOW);
      digitalWrite(seg3Pin,HIGH);
      digitalWrite(seg4Pin,LOW);
      numPrint(3);
      digitalWrite(seg1Pin,LOW);
      digitalWrite(seg2Pin,LOW);
      digitalWrite(seg3Pin,LOW);
      digitalWrite(seg4Pin,HIGH);
      numPrint(4);
    }   
  }
}

void guruguruprint(int num){
  for(int i = 0;i <= 6;i++){
    digitalWrite(pins[i],guruguru[num][i] ? LOW : HIGH);
  }
}

void numPrint(int num){
  for(int i = 0;i <= 6;i++){
    digitalWrite(pins[i], numbers[num][i] ? LOW : HIGH);
  }
}