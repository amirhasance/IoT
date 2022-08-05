#include <Servo.h>
#include <SoftwareSerial.h>

SoftwareSerial mySerial(0, 1);

const int LED = 9;

const int LDR = A3;

void setup() {
  mySerial.begin(9600);
  
  pinMode(LED, OUTPUT);
  pinMode(LDR, INPUT);
}

void loop() {
  int ldr_value = analogRead(LDR);
  int led_volt = map(ldr_value, 1023, 0, 255, 0);
  analogWrite(LED, led_volt);
  mySerial.println(String( "LDR := " + String(ldr_value)  +  "    LED_VOLT := "  + String(led_volt)));

  
}
