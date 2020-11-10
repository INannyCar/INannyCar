// header
#include<Servo.h>

// instance value
Servo servo;

// constant number
const int motor_delay = 300;

// varilable value
unsigned long duration = 0; // ultrasonic wave duration
float distance = 0; // distance to object
int angle = 0; // servo_motor angle

// sensor Pin_num
int trigPin = 12;
int echoPin = 11;
int servoPin = 9;

void setup() {
  
  // ultrasonic_sensor_setup
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  
  // servo_motor_setup
  servo.attach(servoPin);
  
  Serial.begin(9600);
}

void loop() {

  // Ultrasonic Pin reset
  digitalWrite(trigPin, LOW);
  digitalWrite(echoPin, LOW);
  delay(2); // 0.002 sec
  
  // Shoot trig wave
  digitalWrite(trigPin, HIGH);
  delay(10); // 0.01 sec
  digitalWrite(trigPin, LOW);
  
  // The time it takes to send and recive sound waves
  duration = pulseIn(echoPin, HIGH);

  // Calculate distance
  distance = ((float)(340 * duration) / 10000) / 2;
  
  // Print distance
  Serial.print(duration);
  Serial.print("\nDistance : ");
  Serial.print(distance);
  Serial.print("cm\n");

  // Servo control
  /*
  if(distance > 100.00)
  {
    Serial.print("danger\n");
    angle = 180; // Clockwise
    servo.write(angle);
    delay(motor_delay);
    angle = 90; // Stop
    servo.write(angle);
  }
  else
  {
    Serial.print("safe\n");
    angle = 0; // reset
    servo.write(angle);
  }
  */
  delay(500); // Loop delay
}
