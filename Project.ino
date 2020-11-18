// header
#include <SoftwareSerial.h>
#include<Servo.h>

// instance value
SoftwareSerial mySerial(2, 3); // RX, TX
Servo servo1;
Servo servo2;

// constant number
const int motor_delay = 300;

// varilable value
int angle = 0; // servo_motor angle
int incomingByte = 0;
const int bSize = 128; 
char Buffer[bSize];
char Data[bSize];
int ByteCount;

void setup() {
  
  servo1.attach(9); 
  servo2.attach(8);
  // servo_motor_setup
  Serial.begin(9600);
  mySerial.begin(9600);
}

void loop() {
  ByteCount = -1;
  ByteCount =  mySerial.readBytesUntil('\0',Buffer,bSize);  
  // Servo control

  if(ByteCount > 0){
     Serial.println(Buffer);
    if(Buffer == "ON")
    {
      Serial.print("Break on\n");
        angle = 180;
        servo1.write(angle);
        servo2.write(angle);
        delay(300);
        angle = 90;
        servo1.write(angle);
        servo2.write(angle);
    }
    if(Buffer == "OFF")
    {
      Serial.print("Break off\n");
        angle = 0;
        servo1.write(angle);
        servo2.write(angle);
        delay(300);
        angle = 90;
        servo1.write(angle);
        servo2.write(angle);
    }
    Serial.flush();
  }
  delay(500); // Loop delay
}
