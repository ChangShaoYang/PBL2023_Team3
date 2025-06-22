
//header file
#include <Pixy2.h>
#include <Pixy2CCC.h>
#include <VarSpeedServo.h> 

Pixy2 pixy;
VarSpeedServo myservo1;

//motor control mode pin
#define ENA 13
#define IN1 12
#define IN2 11
#define IN3 10
#define IN4 9
#define ENB 8

//servo motor PWM pin
#define servo1 6

//ultrasonic mode pin
#define L_trigger 30
#define L_echo 31
#define R_trigger 42
#define R_echo 43
#define B_trigger 26
#define B_echo 27

//collision censor pin
#define collision 46

//wheel speed
#define speedleft 86 //76
#define speedright 68  //50


//defining variables
int L_cm = 0, R_cm = 0, B_cm = 0;
long R_duration, L_duration, B_duration;
const int turn_Distance = 40;

uint16_t blocks;
int sig[3], x[3], y[3], width[3];
int red_block, white_block;

int dumped = 0;
int pos = 0;
bool walled = false;
int collisionData;


void go_back(){
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, speedleft + 18);
  analogWrite(ENB, speedright + 8); //10
}

void stop_motor(){
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, HIGH);
}

void turn_right(){
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, speedleft - 6);
  analogWrite(ENB, speedright - 6);
}

void turn_left(){
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENA, speedleft - 6);
  analogWrite(ENB, speedright - 6);
}

void go_straight(){
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENA, speedleft + 10);
  analogWrite(ENB, speedright + 10); //12
}

void collision_check(){
  collisionData = digitalRead(collision);//Read data from the sensor using digitalread function
}

void collect_redball(){ //put claw down to collect the ball and then go back to find the wall, finally dumps the balls in.
  stop_motor();
  delay(1000);
  //catch red ball
  
  myservo1.write(1, 200, true); //(1, 250, true)
  delay(2000);
  myservo1.write(60, 50, true); 
  delay(1000);
  collision_check(); //refreash
  //try to find the wall
  while(collisionData == LOW)
  {
    collision_check(); //check
    dumped += 1;
    Serial.println(dumped);
    go_back();
    delay(500);
    if(collisionData == HIGH){ //If switch is pressed
      break;
    }
    if(dumped >= 14){
      break;
    }
  }
  collision_check(); //refreash

  stop_motor();
  delay(300);
  //throw the balls out
  myservo1.write(130, 150, true);
  delay(1200);
  myservo1.write(100, 55, true);

  //go straight to find other red balls
  delay(200);
  go_straight();
}


void Distance(){ //calculate the distance between the ultrasonic sensor and the wall)
  digitalWrite(L_trigger, LOW);
  delayMicroseconds(5);
  digitalWrite(L_trigger, HIGH);
  delayMicroseconds(10);
  digitalWrite(L_trigger, LOW);

  pinMode(L_echo, INPUT);
  L_duration = pulseIn(L_echo, HIGH);
 
  L_cm = (L_duration/2) / 29.1; //L_cm = the distancce between the Left ultrasonic sensor and the wall

  digitalWrite(R_trigger, LOW);
  delayMicroseconds(5);
  digitalWrite(R_trigger, HIGH);
  delayMicroseconds(10);
  digitalWrite(R_trigger, LOW);

  pinMode(R_echo, INPUT);
  R_duration = pulseIn(R_echo, HIGH);
 
  R_cm = (R_duration/2) / 29.1; //R_cm = the distancce between the right ultrasonic sensor and the wall

  digitalWrite(B_trigger, LOW);
  delayMicroseconds(5);
  digitalWrite(B_trigger, HIGH);
  delayMicroseconds(10);
  digitalWrite(B_trigger, LOW);

  pinMode(B_echo, INPUT);
  B_duration = pulseIn(B_echo, HIGH);
 
  B_cm = (B_duration/2) / 29.1; //B_cm = the distancce between the back ultrasonic sensor and the wall
}

void detect_redball(){ //see if there's red ball in sight
  pixy.ccc.getBlocks();
  blocks = pixy.ccc.numBlocks; //blocks = the quantity of the balls

  if(blocks > 0){ //object detected
    if(blocks > 3){ //upmost three objects at a time
      blocks = 3;
    }
    int i = 0;
    while(i < blocks){ //check all the balls
      sig[i] = pixy.ccc.blocks[i].m_signature;
      if(sig[i]==1){ //check if the ball is red
        red_block = i; //define which ball is red
        break;
      }
      else{
        red_block = 10; //if there's no red ball then set red_block equals a number greater than 3(so we don't accidently detect this ball)
      }
      i++;
    }
  }
  else{
    go_straight(); //if no object's detected then just go straight
  }
  
}

void chase_collect_redball(){ //make the car go towards the red ball
  if(red_block < 3){ //if the car find the red ball
      x[red_block] = pixy.ccc.blocks[red_block].m_x; //measure the x of the red ball
      y[red_block] = pixy.ccc.blocks[red_block].m_y; //measure the y of the red ball
      width[red_block] = pixy.ccc.blocks[red_block].m_width;  //measure the width of the red ball(to measure the distance between the car and the red balll)

    if(y[red_block] > 80){
      if(x[red_block] > 120 && x[red_block] < 168){ //if the red ball is in the center of the sight //125 //163
        if(width[red_block] < 41){ //if the red ball is too far away from the car //70
          go_straight(); //go towards the red ball
          delay(15);
        }else if(width[red_block] > 50){
          go_back();
          delay(25);
        }else{ //if the red ball is NEITHER too far away nor too close from the car
          stop_motor();
          int i = 0;
          while(i < blocks){
            sig[i] = pixy.ccc.blocks[i].m_signature;
            if(sig[i]==2){ //check if the ball is white
              white_block = i; //define which ball is white
              break;
            }
            else{
              white_block = 10 ; //if there's no white ball then set white_block equals a number greater than 3(so we don't accidently detect this ball)
            }
            i++;
          }
          if(white_block < 3){ //if the car find the white ball
            width[white_block] = pixy.ccc.blocks[red_block].m_width; 
            if(width[white_block] < 20){ // just take the red ball because the white ball is really far away //50
              collect_redball();
            }
            else{ //if NOT then go straight
              go_straight();
              delay(1000);
              stop_motor();
              delay(1000);
              go_straight();
            }
          }
          else{ //if there's no white ball
            collect_redball();

            Distance(); //calculate the distance
          }
        }
      }
      else if(x[red_block] < 120){ //if the red ball is on the left side of the car //125
        turn_left();
        delay(20);
      }
      else if(x[red_block] > 168){ //if the red ball is on the right side of the car //163
        turn_right();
        delay(20);
      }
    }
  }
    else{
      go_straight(); //the ball is neither on the right side nor on the left side, so just go straight
    }
}

void setup() {
  //defining pins
  Serial.begin(9600);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  pinMode(L_trigger, OUTPUT);
  pinMode(R_trigger, OUTPUT);
  pinMode(B_trigger, OUTPUT);  
  pinMode(L_echo, INPUT);
  pinMode(R_echo, INPUT);
  pinMode(B_echo, INPUT);

  pinMode(collision, INPUT_PULLUP);
  
  pinMode(servo1, OUTPUT);
  myservo1.attach(servo1);
  myservo1.write(100, 100, true);
  delay(1000);
  
  pixy.init();
}

void loop(){
  detect_redball(); //see if there's red ball in sight
  if(blocks && red_block < 3){ //if the car finds the red ball
    chase_collect_redball(); //move towards the red ball and collect it
  }
  else{ //if the car DIDN'T find the red ball
    Distance(); //calculate the distance between ultrasonic sensor and the wall
    if(L_cm < turn_Distance || R_cm < turn_Distance){ //if the right side of the car is closer to the wall than its left side (in the middle)
      if(R_cm <= L_cm){
        //turn left to move away from the wall
        int i = 0;
        while(i < 2){
          go_back();
          delay(200);
          turn_left();
          delay(500);
          stop_motor();
          delay(100);
          detect_redball();
          if(blocks && red_block < 3){ //check if it was chasing the red ball before bumping into the wall, if yes then keep chasing the ball
            chase_collect_redball();
            break;
          }
          i++;
        }
        go_straight();
        /*stop_motor();*/
      }else if (R_cm > L_cm){ //if the left side of the car is closer to the wall than it's right side
        int i = 0;
        while(i < 2){
          go_back();
          delay(200);
          turn_right();
          delay(500);
          stop_motor();
          delay(100);
          detect_redball();
          if(blocks && red_block < 3){ //check if it was chasing the red ball before bumping into the wall, if yes then keep chasing the ball
            chase_collect_redball();
            break;
          }
          i++;
        }
        go_straight();
        /*
        /*stop_motor();*/
      }
    }
  }
}