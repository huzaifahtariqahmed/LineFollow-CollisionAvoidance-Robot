#include <Servo.h>


#define enA 4//Enable1 L298 Pin enA 
#define in1 9 //Motor1  L298 Pin in1 
#define in2 8 //Motor1  L298 Pin in1 
#define in3 7 //Motor2  L298 Pin in1 
#define in4 6 //Motor2  L298 Pin in1 
#define enB 5 //Enable2 L298 Pin enB 

#define L_S A0 //ir sensor Left
#define R_S A1 //ir sensor Right

#define echo A2    //Echo pin
#define trigger A3 //Trigger pin
Servo servo_load;

int Set=15;
bool swit=0;//0 to stay in manual mode, 1 to stay in auto mode
int distance_L, distance_F, distance_R; 

void setup(){ // put your setup code here, to run once

Serial.begin(9600); // start serial communication at 9600bps

pinMode(R_S, INPUT); // declare if sensor as input  
pinMode(L_S, INPUT); // declare ir sensor as input

pinMode(echo, INPUT );// declare ultrasonic sensor Echo pin as input
pinMode(trigger, OUTPUT); // declare ultrasonic sensor Trigger pin as Output  

pinMode(enA, OUTPUT); // declare as output for L298 Pin enA 
pinMode(in1, OUTPUT); // declare as output for L298 Pin in1 
pinMode(in2, OUTPUT); // declare as output for L298 Pin in2 
pinMode(in3, OUTPUT); // declare as output for L298 Pin in3   
pinMode(in4, OUTPUT); // declare as output for L298 Pin in4 
pinMode(enB, OUTPUT); // declare as output for L298 Pin enB 

analogWrite(enA, 220); // Write The Duty Cycle 0 to 255 Enable Pin A for Motor1 Speed 
analogWrite(enB, 220); // Write The Duty Cycle 0 to 255 Enable Pin B for Motor2 Speed 
servo_load.attach(11);

servo_load.write(100);
delay(300);
distance_F = Ultrasonic_read();

}

void manual()
{
  while(1)
  {if (Serial.available() > 0) 
  {
    // Update_Ultra_Sonic();
    Stop();
    char inputvalue = char(Serial.read());
    if (inputvalue == 'F') 
    {
        forword();
    }
    
    if (inputvalue == 'B') 
    {
      backword();
    }
    if (inputvalue == 'R') 
    {
        turnRight();
    } 

    if (inputvalue == 'L') 
    {
        turnLeft();
    } 
    if (inputvalue == 'W')//character from bluetooth which helps shift to auto mode 
    {
      swit=1;
      break;
    } 
  }
  }
}


void loop(){  
  if (swit==0)
  {
    manual();
  }
  else{
//==============================================
//     Line Follower and Obstacle Avoiding
//==============================================  

distance_F = Ultrasonic_read();
Serial.print("D F=");Serial.println(distance_F);


//if Right Sensor and Left Sensor are at White color then it will call forword function
 if((digitalRead(R_S) == 0)&&(digitalRead(L_S) == 0)){
  if(distance_F > Set){forword();}
   else{Check_side();}  
 }  
 
//if Right Sensor is Black and Left Sensor is White then it will call turn Right function
else if((digitalRead(R_S) >0)&&(digitalRead(L_S) == 0)){turnLeft();delay(27);}  

//if Right Sensor is White and Left Sensor is Black then it will call turn Left function
else if((digitalRead(R_S) ==00)&&(digitalRead(L_S) >0)){turnRight();delay(27);} 
    
else if((digitalRead(R_S) >0)&&(digitalRead(L_S) >0)){
  servo_load.write(0);
  delay(300);
  while(1){
    Stop();
  }
  }    
delay(1);

  }
}



//*******Ultrasonic_read*********
long Ultrasonic_read(){
  digitalWrite(trigger, LOW);
  delayMicroseconds(2);
  digitalWrite(trigger, HIGH);
  delayMicroseconds(10);
  long time = pulseIn (echo, HIGH);
  return time / 29 / 2;
}

void compareDistance(){//left 300, right 350
  //   if(distance_L > distance_R){
  // turnLeft();
  // delay(280);
  // forword();
  // delay(450);
  // turnRight();
  // delay(350);
  // forword();
  // delay(450);
  // turnRight();
  // delay(350);
  // forword();
  // delay(450);
  // turnLeft();
  // delay(220);
  // }
  // else{
  turnRight();
  delay(450);
  forword();
  delay(370);
  turnLeft();
  delay(450);
  forword();
  delay(370);  
  turnLeft();
  delay(450);
  forword();
  delay(370);
  turnRight();
  delay(450); 
  forword();
  delay(370);
  // }
}

void Check_side(){
    Stop();
    delay(100);
    compareDistance();
}

void forword(){  //forword
digitalWrite(in1, LOW); //Left Motor backword Pin 
digitalWrite(in2, HIGH); //Left Motor forword Pin 
digitalWrite(in3, HIGH); //Right Motor forword Pin 
digitalWrite(in4, LOW); //Right Motor backword Pin 
}


void backword(){ //backword
digitalWrite(in1, HIGH); //Left Motor backword Pin 
digitalWrite(in2, LOW); //Left Motor forword Pin 
digitalWrite(in3, LOW); //Right Motor forword Pin 
digitalWrite(in4, HIGH); //Right Motor backword Pin 
}

void turnRight(){ //turnRight
digitalWrite(in1, LOW); //Left Motor backword Pin 
digitalWrite(in2, HIGH); //Left Motor forword Pin 
digitalWrite(in3, LOW); //Right Motor forword Pin 
digitalWrite(in4, HIGH); //Right Motor backword Pin 
}

void turnLeft(){ //turnLeft
digitalWrite(in1, HIGH); //Left Motor backword Pin 
digitalWrite(in2, LOW); //Left Motor forword Pin 
digitalWrite(in3, HIGH); //Right Motor forword Pin 
digitalWrite(in4, LOW); //Right Motor backword Pin 
}

void Stop(){ //stop
digitalWrite(in1, LOW); //Left Motor backword Pin 
digitalWrite(in2, LOW); //Left Motor forword Pin 
digitalWrite(in3, LOW); //Right Motor forword Pin 
digitalWrite(in4, LOW); //Right Motor backword Pin 
}