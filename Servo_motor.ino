#include <Servo.h>
Servo servo1;
Servo servo2;
int angle = 0;

void setup() 
{ 
  servo1.attach(9); 
  servo2.attach(8);
  Serial.begin(9600);
} 

void loop() 
{ 
      //angle = 180;//stop
      //servo.write(angle);
      //delay(1000);
      //angle = 90;//시계 
      //servo.write(angle);
      //delay(3000);
      //angle = 0;//반시계
      //servo.write(angle);
      //delay(10000);
      
  if(1)
  {
    char in_data;
    in_data = Serial.read();
    if(in_data == 'a')
    {
      angle = 0;
      servo1.write(angle);
      servo2.write(angle);
      delay(300);
      angle = 90;
      servo1.write(angle);
      servo2.write(angle);
    }
    else if(in_data == 'b')
    {
      angle = 180;
      servo1.write(angle);
      servo2.write(angle);
      delay(300);
      angle = 90;
      servo1.write(angle);
      servo2.write(angle);
    }
  }
} 
