//Header
#include <SoftwareSerial.h>
#include <Servo.h>

//Pin Number
const int Tx_bluetooth = 6;
const int Rx_bluetooth = 7;
const int pinLed = 10;
const int pinTemp = A0;
const int pinIllu = A1;

//Instance Value
SoftwareSerial bluetoothSerial(Tx_bluetooth, Rx_bluetooth);

//Varilable Value
float temperature;
int readingtemp;
int readingillu;
char illutog;
char tog;

void SerialBTParser(void) {
  if(bluetoothSerial.available()) //Bluetooth통신
    {
      tog = bluetoothSerial.read();
      Serial.println(tog);
      if(tog == 'T')
      {
        readingtemp = analogRead(pinTemp);
        temperature = readingtemp / 9.31;
        Serial.println(temperature);
        bluetoothSerial.println(temperature);
      }
      if(tog == 'H')
      {
        digitalWrite(pinLed, HIGH);
      }
      if(tog == 'L')
      {
        digitalWrite(pinLed, LOW);
      }
      if(tog == 'X')
      {
        illutog = tog;
      }
      if(tog == 'Y')
      {
        illutog = tog;
      }
    }
    if(Serial.available())
    {
      bluetoothSerial.write(Serial.read());
    }
    if(illutog == 'X')
    {
      readingillu = analogRead(A1);
      Serial.println(readingillu);
      if(readingillu > 1000)
      {
        digitalWrite(pinLed, HIGH);
        delay(500);
      }
      else
      {
       digitalWrite(pinLed, LOW);
       delay(500);
      }
    }
}

void setup() {
  analogReference(INTERNAL);
  pinMode(pinLed, OUTPUT);
  Serial.begin(9600);
  bluetoothSerial.begin(115200);
}
void loop() {
  SerialBTParser(); 
}
