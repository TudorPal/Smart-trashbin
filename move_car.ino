#include <Servo.h>
#include <NewPing.h>
// Pinii motor 1
#define mpin00 5
#define mpin01 6
// Pinii motor 2
#define mpin10 3
#define mpin11 11

Servo srv;

#define trigPin 4
#define echoPin 2
#define servoPin 8 // attach pin 8 to servo motor

NewPing sonar(trigPin, echoPin, 200);

long duration, distance;
long previousMillis = 0;
long interval = 3000;

char state = 'p';

void setup() {
 // configurarea pinilor motor ca iesire, initial valoare 0
 digitalWrite(mpin00, 0);
 digitalWrite(mpin01, 0);
 digitalWrite(mpin10, 0);
 digitalWrite(mpin11, 0);
 pinMode (mpin00, OUTPUT);
 pinMode (mpin01, OUTPUT);
 pinMode (mpin10, OUTPUT);
 pinMode (mpin11, OUTPUT);
 // pin LED
 pinMode(13, OUTPUT);

 Serial.begin(9600); // Default communication rate of the Bluetooth module
 pinMode(trigPin, OUTPUT); // Sets the trigPin as an OUTPUT
 pinMode(echoPin, INPUT); // Sets the echoPin as an INPUT

 srv.attach(servoPin);
 srv.write(90);
}
// Funcție pentru controlul unui motor
// Intrare: pinii m1 și m2, direcția și viteza
void StartMotor (int m1, int m2, int forward, int speed)
{

 if (speed==0) // oprire
 {
 digitalWrite(m1, 0);
 digitalWrite(m2, 0);
 }
 else
 {
 if (forward)
 {
 digitalWrite(m1, 0);
 analogWrite(m2, speed); // folosire PWM
 }
 else
 {
 digitalWrite(m2, 0);
 analogWrite(m1, speed);
 }
 }
}
// Funcție de siguranță
// Execută oprire motoare, urmată de delay
void delayStopped(int ms)
{
 StartMotor (mpin00, mpin01, 0, 0);
 StartMotor (mpin10, mpin11, 0, 0);
 delay(ms);
}

void loop() {

  unsigned long currentMillis = millis();

 // Bluetooth car movement
 if(Serial.available() > 0){ // Checks whether data is comming from the serial port
  state = Serial.read(); // Reads the data from the serial port
 }
 
 if (state == 'p'){
  delayStopped(0);
 }
 else if (state == 'a') {
  Serial.println("LEFT");
  StartMotor (mpin00, mpin01, 1, 200);
  StartMotor (mpin10, mpin11, 0, 200);
 }
 else if (state == 'd') {
  Serial.println("RIGHT");
  StartMotor (mpin00, mpin01, 0, 200);
  StartMotor (mpin10, mpin11, 1, 200);
 } 
 else if (state == 'w') {
  Serial.println("FORWARD");
  StartMotor (mpin00, mpin01, 1, 200);
  StartMotor (mpin10, mpin11, 1, 280);
 } 
 else if (state == 's') {
  Serial.println("BACKWARD");
  StartMotor (mpin00, mpin01, 0, 280);
  StartMotor (mpin10, mpin11, 0, 200);
 } 
 // Acum se pornesc motoarele DC
 

 // Clears the trigPin condition
  digitalWrite(trigPin, LOW);  // Added this line
  delayMicroseconds(2); // Added this line
  // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10); // Added this line
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  // duration = pulseIn(echoPin, HIGH);
  // distance = duration * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)
  unsigned int uS = sonar.ping();
  distance = uS / US_ROUNDTRIP_CM;

  if(distance < 15)
  {
    previousMillis = currentMillis; 
    srv.write(0);
  } 
  if(currentMillis - previousMillis > interval)
  {
    srv.write(90);
  }

} 