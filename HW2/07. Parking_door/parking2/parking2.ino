#include <Servo.h>
#include <SoftwareSerial.h>

SoftwareSerial terminal1(53, 52);

Servo servo;
int SERVO_PORT = 48;

int pingPin = 43;
int echoPin = 42; 
int LED_RED_PIN = 51;


long passed_time ; 
long time_door_opened;
bool door_is_open = false;
bool isClose = true;
bool is_closing = false;
long max_opening_time = 2 ;
int distance;

void setup() {
  pinMode(LED_RED_PIN, OUTPUT);

  Serial.begin(9600);
  terminal1.begin(9600);
    
  servo.attach(SERVO_PORT);
  servo.write(0);
}

void loop() {

  if(Serial.available() > 0) {
      String command = Serial.readString();
      terminal1.println("recived command : " + command);
      if (command == "1") {
        open_();
      }
      if ( command == "3"){
        close_();
      }
      
  }
 
  if (door_is_open) {
    passed_time = (millis() - time_door_opened) / 1000;
    terminal1.println("door is open for " + String(passed_time));
    if(passed_time > max_opening_time) {
      terminal1.println("time exceeded for door opening");
      close_();
    } 
  }

}

void close_(){
      if (! isClose){
      int distance = getDistance();
      if( distance > 50) {
      terminal1.println(" door is closing") ;
      is_closing = true;
      door_is_open = false;
      isClose = true;
      digitalWrite(LED_RED_PIN, HIGH);
      int prev_value = servo.read();
      terminal1.println("servo angle is " + String(prev_value));
      servo.write(0);
    terminal1.println("door closed ");
      delay(500);
      is_closing = false;
      digitalWrite(LED_RED_PIN, LOW);
    }
      }
  
}
void open_(){
  
  int distance = getDistance();
  if ( distance > 50) {
    terminal1.println("door is opening") ;
    door_is_open = true ;
    isClose = false ;
    is_closing = false;
    digitalWrite(LED_RED_PIN, HIGH);
    int prev_value = servo.read();
    terminal1.println("servo angle is " + String(prev_value));
    for (int i = prev_value ; i < 180 ; i++){
      servo.write(i);
    }
    terminal1.println("door opened ");
    time_door_opened = millis();
    delay(500);
    digitalWrite(LED_RED_PIN, LOW);
     
  }
}


int getDistance() {
  long duration, inches, cm;
  pinMode(pingPin, OUTPUT);
  digitalWrite(pingPin, LOW);
  digitalWrite(pingPin, HIGH);
  pinMode(echoPin, INPUT);
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;

  return distance;
}


long microsecondsToInches(long microseconds) {
  return microseconds / 74 / 2;
}

long microsecondsToCentimeters(long microseconds) {
  return microseconds / 29 / 2;
}
