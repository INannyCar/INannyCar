// header
#include <SoftwareSerial.h>
#include<Servo.h>

// pin number
int RX = 6;
int TX = 7;
int left_motor = 8;
int right_motor = 9;

// instance value
SoftwareSerial mySerial(RX, TX); // RX, TX
Servo left_servo;
Servo right_servo;

// constant number
const int motor_delay_break_on = 280;
const int motor_delay_break_off = 200;

// varilable value
int incomingByte = 0;
const int right = 180;
const int left = 0;
const int movestop = 90;
const int bSize = 128; 
char Buffer[bSize];
char value;
int ByteCount;

void change_state(String (&message)[2]) {
  if(message[0].equals("BREAK")){
    if(message[1].equals("ON")){
        left_servo.write(left);
        right_servo.write(right);
        delay(motor_delay_break_on);
        left_servo.write(movestop);
        right_servo.write(movestop);
    }
    else if(message[1].equals("OFF")){
        left_servo.write(right);
        right_servo.write(left);
        delay(motor_delay_break_off);
        left_servo.write(movestop);
        right_servo.write(movestop);
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
  
  left_servo.attach(left_motor); 
  right_servo.attach(right_motor);
  // servo_motor_setup
  Serial.begin(9600);
  mySerial.begin(9600);
}

void loop() {
  SerialParser();
}
