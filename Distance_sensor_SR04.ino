// sensor pin number
int trigpin = 12;
int echopin = 11;

void setup() {
  pinMode(trigpin, OUTPUT);
  pinMode(echopin, INPUT);
  Serial.begin(9600);
}

void loop() {
  // Shoot trig wave
  digitalWrite(trigpin, HIGH);
  delay(10); // 0.01 sec
  digitalWrite(trigpin, LOW);
  
  // The time it takes to send and recive sound waves
  float duration = pulseIn(echopin, HIGH);
  
  // Calculate distance
  //float distance = ((float)(340 * duration) / 10000) / 2;
  float distance = duration / 29 / 2;
  
  // Print distance
  Serial.print("Distance : ");
  Serial.print(distance);
  Serial.print("cm\n");
  
  // Delay 1sec
  delay(500);
}
