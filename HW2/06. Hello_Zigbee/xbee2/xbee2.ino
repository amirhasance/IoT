#include <SoftwareSerial.h>

SoftwareSerial terminal1(53, 52);

int SOUNDER_PIN = 49;
int GREEN_LED_PIN = 50;
int RED_LED_PIN = 51;

String pass = "9731029";

void setup() {
  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(GREEN_LED_PIN, OUTPUT);
  pinMode(SOUNDER_PIN, OUTPUT);

  terminal1.begin(9600);
  Serial.begin(9600);

  digitalWrite(RED_LED_PIN, HIGH);
  digitalWrite(GREEN_LED_PIN, LOW);
}

void loop() {
  if (Serial.available() > 0) {
      String got_key = Serial.readString();
      terminal1.println(got_key);
      
      if (got_key == pass) {
          digitalWrite(RED_LED_PIN, LOW);
          digitalWrite(GREEN_LED_PIN, HIGH);
          tone(SOUNDER_PIN, 1000);
          delay(3000);
          noTone(SOUNDER_PIN);
          digitalWrite(RED_LED_PIN, HIGH);
          digitalWrite(GREEN_LED_PIN, LOW);}
    }
}
