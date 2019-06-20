#include <QTRSensors.h>

QTRSensorsAnalog qtr((char[]) {5,4,3,2,1,0},6);

#define lma 4
#define lmb 5
#define pwml 10

#define rma 3
#define rmb 2
#define pwmr 9

#define r_trigpin 13
#define r_echopin 12

#define maxspeed 200
#define minspeed 0
#define basespeed 110

#define w_l_maxspeed 150
#define w_l_minspeed 0
#define w_l_basespeed 110

#define w_maxspeed 150
#define w_minspeed 0
#define w_basespeed 110

#define IR0 (analogRead(A0)>500 ? 1 : 0) // check our value for black
#define IR1 (analogRead(A1)>500 ? 1 : 0)
#define IR2 (analogRead(A2)>500 ? 1 : 0)
#define IR3 (analogRead(A3)>500 ? 1 : 0)
#define IR4 (analogRead(A4)>500 ? 1 : 0)
#define IR5 (analogRead(A5)>500 ? 1 : 0)


int lastError = 0;
int M1 = basespeed;
int M2 = basespeed;

int w_l_M1 = w_l_basespeed;
int w_l_M2 = w_l_basespeed;

int w_lastError = 0;
int w_M1 = w_basespeed;
int w_M2 = w_basespeed;

int wall_count=0;
long duration;
int distance;

int isObstaclePin = 0;  //  input pin
int isObstacle = HIGH;  // HIGH MEANS NO OBSTACLE

void setup() {
  pinMode(r_trigpin,OUTPUT);  
  pinMode(r_echopin,INPUT); 
  pinMode(isObstaclePin, INPUT);
 
  pinMode(isObstaclePin, INPUT);
  //Serial.begin(9600);
  
  calibratte();
  Serial.begin(9600);
}

void loop() { 
//  Serial.print(IR0); // Serial print Left most sensor value
//  Serial.print(" / ");
//  Serial.println(IR5); // Serial print Right most sensor value

line_follow();

}


void forward(int lspd, int rspd){
  digitalWrite(lma,HIGH);
  digitalWrite(lmb,LOW);
  digitalWrite(rma,HIGH);
  digitalWrite(rmb,LOW);
  analogWrite(pwml,lspd);
  analogWrite(pwmr,rspd);
}

void reverse(int spd){
  digitalWrite(lma,LOW);
  digitalWrite(lmb,HIGH);
  digitalWrite(rma,LOW);
  digitalWrite(rmb,HIGH);
  analogWrite(pwml,spd);
  analogWrite(pwmr,spd);
}

void breake()
{
  digitalWrite(lma,LOW);
  digitalWrite(lmb,LOW);
  digitalWrite(rma,LOW);
  digitalWrite(rmb,LOW);
}

void calibratte()
{
  for(int i=0;i<100;i++)
  {
     qtr.calibrate();
     delay(5);
  }
}

void line_follow()
{
    unsigned int sensors[6];
    qtr.readLine(sensors, QTR_EMITTERS_ON, 1);
    int position = qtr.readLine(sensors);
    //Serial.println(position);
    int error = position - 2500;
    int motorSpeed = 0.2 * error + 0.4 * (error - lastError);
    lastError = error;
    int m1Speed = M1 + motorSpeed;
    int m2Speed = M2 - motorSpeed;
   
    if (m1Speed < 0)
      m1Speed = minspeed;
    else if(m1Speed > 255)
      m1Speed = maxspeed;
    if (m2Speed < 0)
      m2Speed = minspeed;
    else if(m2Speed>255)
      m2Speed = maxspeed;
    //Serial.println(position);
    digitalWrite(lma,HIGH);
    digitalWrite(lmb,LOW);
    digitalWrite(rma,HIGH);
    digitalWrite(rmb,LOW);
    analogWrite(pwml,m1Speed);
    analogWrite(pwmr,m2Speed);

  
}

void line_follow_w_l() // White line, Black background 
{
    unsigned int sensors[6];
    qtr.readLine(sensors, QTR_EMITTERS_ON, 1);
    int position = qtr.readLine(sensors);
    //Serial.println(position);
    int error = 2500 - position;
    int motorSpeed = 2 * error + 4 * (error - lastError);
    lastError = error;
    int m1Speed = w_l_M1 + motorSpeed;
    int m2Speed = w_l_M2 - motorSpeed;
   
    if (m1Speed < 0)
      m1Speed = w_l_minspeed;
    else if(m1Speed > 255)
      m1Speed = w_l_maxspeed;
    if (m2Speed < 0)
      m2Speed = w_l_minspeed;
    else if(m2Speed>255)
      m2Speed = w_l_maxspeed;
    //Serial.println(position);
    digitalWrite(lma,HIGH);
    digitalWrite(lmb,LOW);
    digitalWrite(rma,HIGH);
    digitalWrite(rmb,LOW);
    analogWrite(pwml,m1Speed);
    analogWrite(pwmr,m2Speed);
  
}

void wall_follow()
{ 
 /* if(!fir)
    {
      wall_count++;
     // turing code goes here
        while(get_dist() < 10)
        {
        digitalWrite(lma,HIGH);
        digitalWrite(lmb,LOW);
        digitalWrite(rma,LOW);
        digitalWrite(rmb,HIGH);
        analogWrite(pwml,80);
        analogWrite(pwmr,80);
        }
     }*/


      int dist = get_dist();
      int error = dist - 7 ;
      int motorSpeed = 10 * error + 20 * (error - lastError);
      w_lastError = error;
      int m1Speed = w_M1 + motorSpeed;
      int m2Speed = w_M2 - motorSpeed;
     
      if (m1Speed < 0)
        m1Speed = w_minspeed;
      else if(m1Speed > 255)
        m1Speed = w_maxspeed;
      if (m2Speed < 0)
        m2Speed = w_minspeed;
      else if(m2Speed>255)
        m2Speed = w_maxspeed;
  
      digitalWrite(lma,HIGH);
      digitalWrite(lmb,LOW);
      digitalWrite(rma,HIGH);
      digitalWrite(rmb,LOW);
      analogWrite(pwml,m1Speed);
      analogWrite(pwmr,m2Speed);
      //Serial.print("\n Left = ");
      //Serial.print(m1Speed);
      //Serial.print("\t Right = ");
      //Serial.print(m2Speed);
  
}

int get_dist()
{
     digitalWrite(r_trigpin, LOW);
     delayMicroseconds(2);
   
     digitalWrite(r_trigpin, HIGH);
     delayMicroseconds(10);
     digitalWrite(r_trigpin, LOW);
    
     duration = pulseIn(r_echopin, HIGH);
     distance= duration*0.034/2;

     return distance;
 
}

