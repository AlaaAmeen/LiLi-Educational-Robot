#include <Keypad.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#define IN1 12
#define IN2 13
#define EN1 10
#define IN3 3
#define IN4 4
#define EN2 11
#define pi 3.14159
#define L 0.13 //meter
#define maxVoltR 4.5
#define maxVoltL 4.6

LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display
int flag = 1;
int X = 0, Y = 0, Xold = 0, Yold = 0, Xdiff = 0, Ydiff = 0, counter = 1, cursorPlace = 11;

byte B[8] = {
  0b00000,
  0b00000,
  0b00000,
  0b10001,
  0b11111,
  0b00000,
  0b00100,
  0b00000
};
byte T[8] = {
  0b00000,
  0b01010,
  0b00000,
  0b10001,
  0b11111,
  0b00000,
  0b00000,
  0b00000
};
byte H[8] = {
  0b00000,
  0b01110,
  0b00100,
  0b01000,
  0b10000,
  0b01000,
  0b00110,
  0b00000
};
byte Kh[8] = {
  0b00100,
  0b00000,
  0b01110,
  0b00100,
  0b01000,
  0b10000,
  0b01000,
  0b00110
};
byte D[8] = {
  0b00000,
  0b00000,
  0b00001,
  0b00001,
  0b00001,
  0b01111,
  0b00000,
  0b00000
};
byte TH[8] = {
  0b00001,
  0b00000,
  0b00001,
  0b00001,
  0b00001,
  0b01111,
  0b00000,
  0b00000
};
byte R[8] = {
  0b00000,
  0b00001,
  0b00001,
  0b00001,
  0b00010,
  0b00100,
  0b01000,
  0b00000
};
byte Z[8] = {
  0b00001,
  0b00000,
  0b00001,
  0b00001,
  0b00001,
  0b00010,
  0b00100,
  0b01000
};

const byte ROWS = 4; //four rows
const byte COLS = 4; //three columns
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte rowPins[ROWS] = {9, 8, 7, 6}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {5, 2, 1, 0}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

void setup(){
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(EN1, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(EN2, OUTPUT);
  //Serial.begin(9600);
  lcd.init();          // initialize the lcd 
  lcd.backlight();
  lcd.createChar(0,B);
  lcd.createChar(1,T);
  lcd.createChar(2,H);
  lcd.createChar(3,Kh);
  lcd.createChar(4,D);
  lcd.createChar(5,TH);
  lcd.createChar(6,R);
  lcd.createChar(7,Z);
  lcd.begin(16,2);
}
  
void loop(){
  char key = keypad.getKey();

   if (key == 'A'){
     X = 0;
     Y = 0;
     steps(X, Y);
   }
   else if (key == '3'){
     X = 1;
     Y = 0;
     steps(X, Y);
     BLetter();
   }
   else if (key == '2'){
     X = 2;
     Y = 0;
     steps(X, Y);
     TLetter();
   }
   else if (key == '1'){
     X = 3;
     Y = 0;
     steps(X, Y);
   }
   else if (key == 'B'){
     X = 0;
     Y = 1;
     steps(X, Y);
   }
   else if (key == '6'){
     X = 1;
     Y = 1;
     steps(X, Y);
     HLetter();
   }
   else if (key == '5'){
     X = 2;
     Y = 1;
     steps(X, Y);
     KhLetter();
   }
   else if (key == '4'){
     X = 3;
     Y = 1;
     steps(X, Y);
     DLetter();
   }
   else if (key == 'C'){
     X = 0;
     Y = 2;
     steps(X, Y);
     THLetter();
   }
   else if (key == '9'){
     X = 1;
     Y = 2;
     steps(X, Y);
     RLetter();
   }
   else if (key == '8'){
     X = 2;
     Y = 2;
     steps(X, Y);
     ZLetter();
   }
   else if (key == '7'){
     X = 3;
     Y = 2;
     steps(X, Y);
   }
   else if (key == 'D'){
     reset();
     X = Y = Xold = Yold = Xdiff = Ydiff = 0;
     flag = 1;
     counter = 1;
     cursorPlace = 11;
     lcd.clear();
   }
    
}

/*
  float PWM_Evaluation(float V, float W){
  float rightVelocity, leftVelocity, voltR, voltL;
  int pwmR, pwmL;

  if (counter == 1){
    rightVelocity = (V + (W*L/2.0));
    voltR = (0.2042 * pow(rightVelocity,2)) + (6.4 * rightVelocity) + 0.59;
    //voltR = (1177.9 * pow(rightVelocity,3)) - (1630.6 * pow(rightVelocity,2)) + (751.16 * rightVelocity) - 111.31;
    pwmR = (int)((voltR / maxVoltR) * 255);
    counter = 0;
    return pwmR;
  }

  else{
    leftVelocity = (V - (W*L/2.0));
    voltL = (0.2047 * pow(leftVelocity,2)) + (6.2452 * leftVelocity) + 0.4473;
    //voltL = (248.21 * pow(leftVelocity,3)) - (342.91 * pow(leftVelocity,2)) + (161.29 * leftVelocity) - 21.791;
    pwmL = (int)((voltL / maxVoltL) * 255);
    counter = 1;
    return pwmL;
  }
}
 */
void BLetter(){
  lcd.setCursor(cursorPlace,0);
  cursorPlace -= 1;
  lcd.write(0);
}
void TLetter(){
  lcd.setCursor(cursorPlace,0);
  cursorPlace -= 1;
  lcd.write(1);
}
void HLetter(){
  lcd.setCursor(cursorPlace,0);
  cursorPlace -= 1;
  lcd.write(2);
}
void KhLetter(){
  lcd.setCursor(cursorPlace,0);
  cursorPlace -= 1;
  lcd.write(3);
}
void DLetter(){
  lcd.setCursor(cursorPlace,0);
  cursorPlace -= 1;
  lcd.write(4);
}
void THLetter(){
  lcd.setCursor(cursorPlace,0);
  cursorPlace -= 1;
  lcd.write(5);
}
void RLetter(){
  lcd.setCursor(cursorPlace,0);
  cursorPlace -= 1;
  lcd.write(6);
}
void ZLetter(){
  lcd.setCursor(cursorPlace,0);
  cursorPlace -= 1;
  lcd.write(7);
}

void forward(int diff){
  /*
  float V = 0.36; float W = 0; //with the new battery I decreased the linear velocity
  float PWMR = PWM_Evaluation(V, W);
  float PWML = PWM_Evaluation(V, W);
   */
  delay(500);
  
  digitalWrite(IN1,HIGH);
  digitalWrite(IN2,LOW);
  analogWrite(EN1,165);  // We can adjust this value
  digitalWrite(IN3,HIGH);
  digitalWrite(IN4,LOW);
  analogWrite(EN2,175);   // We can adjust this value
  delay(diff*420);  // We can adjust this value
  analogWrite(EN1, 0);
  analogWrite(EN2, 0);

  delay(300);
}

void backward(int diff){
  /*
  float V = 0.4; float W = 0;
  float PWMR = PWM_Evaluation(V, W);
  float PWML = PWM_Evaluation(V, W);
   */
  delay(500);
  
  digitalWrite(IN1,LOW);
  digitalWrite(IN2,HIGH);
  analogWrite(EN1,165);  // We can adjust this value
  digitalWrite(IN3,LOW);
  digitalWrite(IN4,HIGH);
  analogWrite(EN2,185);   // We can adjust this value
  delay(diff*430);   // We can adjust this value
  analogWrite(EN1, 0);
  analogWrite(EN2, 0);

  delay(300);
}

void turnRight(){
  /*float V = 0.36; float W = -1*(pi/2);   //clockwise
  float PWMR = PWM_Evaluation(V, W);
  float PWML = PWM_Evaluation(V, W);*/
  delay(500);
  
  //Right wheel inputs are LOW
  digitalWrite(IN1,HIGH);
  digitalWrite(IN2,LOW);
  analogWrite(EN1,167); 
  digitalWrite(IN3,LOW);
  digitalWrite(IN4,HIGH);
  analogWrite(EN2,107);   // We can adjust this value
  delay(300);   // We can adjust this value to give 90 degree
  analogWrite(EN1, 0);
  analogWrite(EN2, 0);

  delay(300);
}

void turnLeft(){
  /*
  float V = 0.36; float W = pi/2;  //couter clockwise
  float PWMR = PWM_Evaluation(V, W);
  float PWML = PWM_Evaluation(V, W);
   */
  delay(500);
  
  digitalWrite(IN1,LOW);
  digitalWrite(IN2,HIGH);
  analogWrite(EN1,94); 
  //Left wheel inputs are LOW
  digitalWrite(IN3,HIGH);
  digitalWrite(IN4,LOW);
  analogWrite(EN2,183);   // We can adjust this value
  delay(310);   // We can adjust this value to give 90 degree
  analogWrite(EN1, 0);
  analogWrite(EN2, 0);

  delay(300);
}

void motionInX(int Xdiff, int Ydiff){
  if (Xdiff > 0 && flag == 1){
    forward(abs(Xdiff));
    flag = 1;
  }
  else if (Xdiff < 0 && flag == 1){
    backward(abs(Xdiff));
    //flag = 2;   /////////////////// backward the flag remain the same
  }
  else if (Xdiff > 0 && flag == 3){
    turnRight();
    forward(abs(Xdiff));
    flag = 1;
  }
  else if (Xdiff < 0 && flag == 3){
    turnLeft();
    forward(abs(Xdiff));
    flag = 2;
  }
  else if (Xdiff > 0 && flag == 4){
    turnLeft();
    forward(abs(Xdiff));
    flag = 1;
  }
  else if (Xdiff < 0 && flag == 4){
    turnRight();
    forward(abs(Xdiff));
    flag = 2;
  }
  else if (Xdiff > 0 && flag == 2){
    backward(abs(Xdiff));
    //flag = 2;
  }
  else if (Xdiff < 0 && flag == 2){
    forward(abs(Xdiff));
    flag = 2;
  }
}

void motionInY(int Xdiff, int Ydiff){
  if (Ydiff > 0 && flag == 3){
    forward(abs(Ydiff));
    flag = 3;
  }
  else if (Ydiff < 0 && flag == 3){
    backward(abs(Ydiff));
    //flag = 4;
  }
  else if (Ydiff > 0 && flag == 1){
    turnLeft();
    forward(abs(Ydiff));
    flag = 3;
  }
  else if (Ydiff < 0 && flag == 1){
    turnRight();
    forward(abs(Ydiff));
    flag = 4;
  }
  else if (Ydiff > 0 && flag == 2){
    turnRight();
    forward(abs(Ydiff));
    flag = 3;
  }
  else if (Ydiff < 0 && flag == 2){
    turnLeft();  //////////////////////////////////////////////////////////
    forward(abs(Ydiff));
    flag = 4;
  }
  else if (Ydiff > 0 && flag == 4){
    backward(abs(Ydiff));
    //flag = 4;
  }
  else if (Ydiff < 0 && flag == 4){
    forward(abs(Ydiff));
    flag = 4;
  }
}

void reset(){
  digitalWrite(IN1,LOW);
  digitalWrite(IN2,LOW);
  analogWrite(EN1,0);
  digitalWrite(IN3,LOW);
  digitalWrite(IN4,LOW);
  analogWrite(EN2,0);
  delay(1000);
}

void steps(int X, int Y){
  Xdiff = X - Xold;
  Ydiff = Y - Yold;
  if (Xdiff == 0 && Ydiff == 0){
    reset();
  }
  else{
    if (Xdiff != 0){
      motionInX(Xdiff, Ydiff);
    }
    if (Ydiff != 0){
      motionInY(Xdiff, Ydiff);
    }
  }
  Xold = X;
  Yold = Y;
}
