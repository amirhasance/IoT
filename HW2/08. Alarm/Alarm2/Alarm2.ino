#include <SoftwareSerial.h>

SoftwareSerial terminal1(53, 52);

int alarm_duration;
const int SOUNDER_PIN = 49;
bool time_is_set = false ;
String message ;
long snooze_time = 3000; // after pushing * for snoozing time ;
long alarm_set_time_at;
bool flag = false ;
void setup() {
  terminal1.begin(9600);
  Serial.begin(9600);

  pinMode(SOUNDER_PIN, OUTPUT);
}

void loop() {
  if (Serial.available() > 0 ) {
    message = Serial.readString();
    terminal1.println(message);
    if (time_is_set){
        if ( message == "*") {
          noTone(SOUNDER_PIN);
          alarm_set_time_at = millis() + 5000;
          terminal1.println("now : " + String(millis()));
          terminal1.println("alarm set at : " + String(alarm_set_time_at/1000));
          delay(5000);
        }
        if ( message == "#") {
          noTone(SOUNDER_PIN);
           exit(0);
        }
    }
    else {
       alarm_duration = message.toInt();
       alarm_set_time_at  = millis();
       time_is_set = true ;
       terminal1.println("alarm set at : " + String(alarm_set_time_at/1000));
       terminal1.println("alarm set for : " + String(alarm_duration) + " seconds");
    }
  
  }
  if (time_is_set){
        if ( (millis() - alarm_set_time_at)/1000 >= alarm_duration ) {
          tone(SOUNDER_PIN, 100);
        }
        else {
           noTone(SOUNDER_PIN);
        }
  }

  
}
