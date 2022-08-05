#include <Servo.h>
#include <SoftwareSerial.h>

#define  SERVO_PORT 13
#define ping_pin  7
#define echo_pin  6 

SoftwareSerial mySerial(0, 1);
Servo servo;
int start_time ;

bool flag = false ;

void setup() {
  servo.attach(SERVO_PORT);
  servo.write(0);

  mySerial.begin(9600);
  

 
}

void loop() {

  delay(50);
  pinMode(ping_pin, OUTPUT);
   digitalWrite(ping_pin, LOW);
  digitalWrite(ping_pin, HIGH);
  pinMode(echo_pin, INPUT);
  
  long signal_duration_time = pulseIn(echo_pin, HIGH);
  
  int distance = signal_duration_time * 0.034 / 2;
  mySerial.print("duration time"); 
  mySerial.println(signal_duration_time);

  if(distance <= 50) {
      if (flag == false ) {
          flag = true ;
          start_time = millis();}
      else {
          int proximity_time ;
          proximity_time = (millis() - start_time) / 1000;
          mySerial.print(proximity_time );
          mySerial.print("s ");
          mySerial.println();
          if(proximity_time > 3) {
              servo.write(90);}
          else {
              servo.write(0);
            }
      }
  }
  else{
      flag = false;
      servo.write(0);
  }

  if(signal_duration_time > 0) {
    mySerial.print("distance ") ;
    mySerial.print(distance);
    mySerial.println(" cm");
  }
}
