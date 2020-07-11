#include <Keypad.h>
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

int flag = 1;
int X = 0; int Y = 0; int Xold = 0; int Yold = 0; int Xdiff = 0; int Ydiff = 0; int counter = 1;

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
   }
   else if (key == '2'){
     X = 2;
     Y = 0;
     steps(X, Y);
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
   }
   else if (key == '5'){
     X = 2;
     Y = 1;
     steps(X, Y);
   }
   else if (key == '4'){
     X = 3;
     Y = 1;
     steps(X, Y);
   }
   else if (key == 'C'){
     X = 0;
     Y = 2;
     steps(X, Y);
   }
   else if (key == '9'){
     X = 1;
     Y = 2;
     steps(X, Y);
   }
   else if (key == '8'){
     X = 2;
     Y = 2;
     steps(X, Y);
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
   }
    
}

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

void forward(int diff){
  float V = 0.36; float W = 0;

  float PWMR = PWM_Evaluation(V, W);
  float PWML = PWM_Evaluation(V, W);
  delay(500);
  
  digitalWrite(IN1,HIGH);
  digitalWrite(IN2,LOW);
  //analogWrite(EN1,PWML);  // We can adjust this value
  analogWrite(EN1,150);
  digitalWrite(IN3,HIGH);
  digitalWrite(IN4,LOW);
  //analogWrite(EN2,PWMR);   // We can adjust this value
  analogWrite(EN2,165);
  delay(diff*480);  // We can adjust this value
  analogWrite(EN1, 0);
  analogWrite(EN2, 0);

  delay(300);
}

void backward(int diff){
  float V = 0.4; float W = 0;

  float PWMR = PWM_Evaluation(V, W);
  float PWML = PWM_Evaluation(V, W);
  delay(500);
  
  digitalWrite(IN1,LOW);
  digitalWrite(IN2,HIGH);
  //analogWrite(EN1,PWML);  // We can adjust this value
  analogWrite(EN1,150);
  digitalWrite(IN3,LOW);
  digitalWrite(IN4,HIGH);
  //analogWrite(EN2,PWMR);   // We can adjust this value
  analogWrite(EN2,165);
  delay(diff*550);   // We can adjust this value
  analogWrite(EN1, 0);
  analogWrite(EN2, 0);

  delay(300);
}

void turnRight(){
  float V = 0.36; float W = -1*(pi/2);   //clockwise

  float PWMR = PWM_Evaluation(V, W);
  float PWML = PWM_Evaluation(V, W);
  delay(500);
  
  //Right wheel inputs are LOW
  digitalWrite(IN1,HIGH);
  digitalWrite(IN2,LOW);
  //analogWrite(EN1,PWML); 
  analogWrite(EN1,121);
  digitalWrite(IN3,LOW);
  digitalWrite(IN4,HIGH);
  //analogWrite(EN2,PWMR);   // We can adjust this value
  analogWrite(EN2,127);
  delay(370);   // We can adjust this value to give 90 degree
  analogWrite(EN1, 0);
  analogWrite(EN2, 0);

  delay(300);
}

void turnLeft(){
  float V = 0.36; float W = pi/2;  //couter clockwise

  float PWMR = PWM_Evaluation(V, W);
  float PWML = PWM_Evaluation(V, W);
  delay(500);
  
  digitalWrite(IN1,LOW);
  digitalWrite(IN2,HIGH);
  //analogWrite(EN1,PWML);
  analogWrite(EN1,121); 
  //Left wheel inputs are LOW
  digitalWrite(IN3,HIGH);
  digitalWrite(IN4,LOW);
  //analogWrite(EN2,PWMR);   // We can adjust this value
  analogWrite(EN2,127);
  delay(370);   // We can adjust this value to give 90 degree
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
