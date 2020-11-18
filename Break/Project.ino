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
char value;
char Data[bSize];
int ByteCount;

void change_state(String (&message)[2]) {
  if(message[0].equals("BREAK")){
    if(message[1].equals("ON")){
        angle = 180;
        servo1.write(angle);
        servo2.write(angle);
        delay(300);
        angle = 90;
        servo1.write(angle);
        servo2.write(angle);
    }
    else if(message[1].equals("OFF")){
        angle = 0;
        servo1.write(angle);
        servo2.write(angle);
        delay(300);
        angle = 90;
        servo1.write(angle);
        servo2.write(angle);
    }
  }
}


void SerialParser(void) {
  ByteCount = -1;
  ByteCount =  mySerial.readBytesUntil('\0',Buffer,bSize);  

  if(ByteCount > 0) {                    
    Serial.println(Buffer);
    String message[2];
    spliter(Buffer, ':',message);    
    change_state(message);
        
    memset(Data, 0, bSize);          
  }
  memset(Buffer, 0, bSize);     
  Serial.flush();
}

void spliter(String sData, char cSeparator, String (&message)[2]) {  
  int nGetIndex = 0 ;
  
  String sTemp = "";
  String sCopy = sData;

  while(true)
  {
    nGetIndex = sCopy.indexOf(cSeparator);
    if(-1 != nGetIndex)
    {
      sTemp = sCopy.substring(0, nGetIndex);

      message[0] = sTemp;
      sCopy = sCopy.substring(nGetIndex + 1);
    }
    else
    {
      message[1] = sCopy;
      break;
    }
  }
}

void setup() {
  
  servo1.attach(9); 
  servo2.attach(8);
  // servo_motor_setup
  Serial.begin(9600);
  mySerial.begin(9600);
}

void loop() {
  SerialParser();
  delay(500); // Loop delay
}
