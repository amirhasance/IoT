#define BLYNK_TEMPLATE_ID           "TMPLRzi7eumm"
#define BLYNK_DEVICE_NAME           "Template1"
#define BLYNK_AUTH_TOKEN            "WrcVashICO1FNUzVm0RKkJIrcbgiVSJ6"


// Comment this out to disable prints and save space
#define BLYNK_PRINT Serial1


#include <BlynkSimpleStream.h>
#include <SoftwareSerial.h>
#include<Servo.h>

BlynkTimer timer;
BlynkTimer timer1;
Servo myservo;

SoftwareSerial terminal(53, 52);
char auth[] = BLYNK_AUTH_TOKEN;
int current_temp = 30 ; 
bool is_on = false; 
int temp_must_be ; 
int LED_PORT = 47;
int TEMPERATURE_PORT = A0 ; 

// reads from slider 
BLYNK_WRITE(V2)
{
  int pinValue = param.asInt();
  temp_must_be = pinValue;
  terminal.println("temp is set for : " + String(temp_must_be));
}

// reads from Switch 
void myTimerEvent()
{ 
  
  Blynk.virtualWrite(V3,current_temp );
  current_temp -= 1 ;
  
}

void warming_up_timer(){
   int delta_temp = current_temp - temp_must_be;
   terminal.println("delta tempretaure is  : " + String(delta_temp));
   
   if ( delta_temp < -5 or ( delta_temp < 7 and is_on == true )) {
       is_on = true ;
       digitalWrite(LED_PORT , HIGH);
       myservo.write(180);
       current_temp += 2 ;
       Blynk.virtualWrite(V3,current_temp );
   }

   else {
    is_on = false ; 
    digitalWrite(LED_PORT , LOW);
    myservo.write(0);
   }
}

void set_current_temp(){
  
  int analogValue = analogRead(TEMPERATURE_PORT);
  terminal.println("analogValue : " + String(analogValue));
  int temp = analogValue / 9.35;
  terminal.println("reading temprature : " + String(temp));
  current_temp = temp;
}


void setup()
{
  terminal.println("setting up .. ");
  analogReference(INTERNAL1V1);
  Serial1.begin(115200);
  terminal.begin(9600);
  myservo.attach(30);
  Serial.begin(9600);
  Blynk.begin(Serial, auth);
  pinMode(LED_PORT , OUTPUT);
  timer.setInterval(1000L, myTimerEvent);
  timer.setInterval(500L , warming_up_timer);
  set_current_temp();
 }

void loop()
{
  Blynk.run();
  timer.run();
  timer1.run();
}
