#define BLYNK_TEMPLATE_ID "TMPL-7r1LKMS"
#define BLYNK_DEVICE_NAME "Template2Q10"
#define BLYNK_AUTH_TOKEN "I_SCbmX7jKnG4B9qjg5uwAiDfN1lXdUa"

#define BLYNK_PRINT Serial1


#include <BlynkSimpleStream.h>
#include <SoftwareSerial.h>
#include<Servo.h>

BlynkTimer timer;
Servo myservo;

SoftwareSerial terminal(53, 52);
const int pingPin = 41;
const int echoPin = 40; 
char auth[] = BLYNK_AUTH_TOKEN;
int SOUNDER_PORT = 43;
bool is_on = false;  
int LED_PORT = 47;
int camera_value = 90 ;
int last_distance ; 


// reads from slider  for camera angle 
BLYNK_WRITE(V1)
{
  if (is_on == true ){
    int pinValue = param.asInt();
    camera_value = pinValue;
    terminal.println("camera angle is set for : " + String(camera_value));
    myservo.write(camera_value);
  }
  else {
    terminal.println("system is off , cant turn camera") ;
  }

}

// reads from swtich for ON/OFF System 
BLYNK_WRITE(V0)
{
  int pinValue = param.asInt();
  is_on = pinValue;
  if ( is_on == 1 ) {
    terminal.println("system turned on");
  }
  if ( is_on == 0) {
    terminal.println("system turned_off");
  }
}
// reads from Switch 
void myTimerEvent()
{ 
  int current_distance = getDistance();
  int delta_distance = abs(current_distance - last_distance);
  if (delta_distance > 5 ) {
    tone(SOUNDER_PORT , 100);
    digitalWrite(LED_PORT , HIGH);
  }
  else {
    noTone(SOUNDER_PORT);
  }
  terminal.println("delta distance  = " + String(delta_distance));
  last_distance = current_distance ;

}

int getDistance() {
  long duration, inches, cm;
  pinMode(pingPin, OUTPUT);
  digitalWrite(pingPin, LOW);
  digitalWrite(pingPin, HIGH);
  pinMode(echoPin, INPUT);
  duration = pulseIn(echoPin, HIGH);
  int localDistance = duration * 0.034 / 2;

  return localDistance;
}

void setup()
{
  terminal.println("setting up .. ");
  Serial1.begin(115200);
  terminal.begin(9600);
  myservo.attach(30);
  Serial.begin(9600);
  Blynk.begin(Serial, auth);
  myservo.write(0);
  pinMode(LED_PORT , OUTPUT);
  last_distance = getDistance() ;
  timer.setInterval(500L, myTimerEvent);
 }

void loop()
{
  Blynk.run();
  timer.run();
}
