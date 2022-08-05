#include <Servo.h>
#include <Keypad.h>
#include <LiquidCrystal.h>
#include <SoftwareSerial.h>


const int ROW_NUM = 4;
const int COLUMN_NUM = 3;
char keys[ROW_NUM][COLUMN_NUM] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};
byte pin_rows[ROW_NUM] = {8 , 7 , 6 , 5};
byte pin_column[COLUMN_NUM] = {2 , 3 , 4};
Keypad keypad = Keypad( makeKeymap(keys), pin_rows, pin_column, ROW_NUM, COLUMN_NUM );

SoftwareSerial mySerial(0 , 1);

#define RS_PIN 14
#define EN_PIN 15
#define D4_PIN 16
#define D5_PIN 17
#define D6_PIN 18
#define D7_PIN 19


String class_password = "1401";
LiquidCrystal lcd(RS_PIN, EN_PIN,D4_PIN, D5_PIN,D6_PIN, D7_PIN );


Servo servo;

int LED_PIN = 13;
int BUZZER_PIN = 12;
int SERVO_PIN = 21;

int ALLOWED_TIME = 10;
int ENDS_AT = 20;

String Students[5] = {"1111", "2222", "3333", "4444", "5555"};
String presenceList[5];
String absenseList[5];

uint32_t startTime;
String userType = "";
bool isStarted = false;

void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  
  lcd.begin(16, 4);
  servo.attach(SERVO_PIN);
  mySerial.begin(9600);

  servo.write(0);
}

void loop() {
  uint32_t now_ = millis();
  uint32_t diff = (now_ - startTime)/1000;
   
  char inputKey = keypad.getKey();

  printOnLcd(0, userType);

  if (isStarted) {
     mySerial.println(diff);
  }
  else {
    mySerial.println("Wait to Start Class " ) ;
  }
 

  if(diff > ENDS_AT) {
    printList();
    exit(0);
  }
  
  if(userType ==  class_password && !isStarted) {
    isStarted = true;
    startTime = millis();
    lcd.clear();
    userType = "";
  }
  else if(inputKey == '#') {
    if (isStarted ) {
    printOnLcd(0, "ID: " + userType);
    if(diff <= ALLOWED_TIME) {
      digitalWrite(LED_PIN, HIGH);
      servo.write(180);
      delay(1000);
      digitalWrite(LED_PIN, LOW);
      servo.write(0);
      for(int i=0; i<5; ++i) {
        if(presenceList[i] == "") {
          presenceList[i] = userType;
          userType = "";
        }
      }
    } else {
      printOnLcd(1, "Late for: " + String(diff));
      printOnLcd(2 , "No entry :/");
      tone(BUZZER_PIN, 1000);
      delay(1000);
      noTone(BUZZER_PIN);
      for(int i=0; i<5; ++i) {
        if(absenseList[i] == "") {
          absenseList[i] == userType;
          userType = "";
        }
      }
    }
    lcd.clear();
  }
  else {
    userType  = "" ; 
    printOnLcd(2  , "class not started :( " ) ;
    printOnLcd( 3 , "wait for Professor") ;
    delay(1000);
    lcd.clear();
    
  }
  } else {
    userType += String(inputKey);  
  }
}

bool array_contains(String *array_, String value) {
  for(int i=0; i<9; ++i) {
    if(array_[i] == value) {
      return true;
    }  
  }
  return false;
}

void printOnLcd(int lineNo, String Message) {
  lcd.setCursor(0, lineNo);
  lcd.print(Message);
}

void printList() {
  mySerial.println();
  mySerial.println();
  mySerial.println(" Paticipants :");
  for(int i=0; i< 5; ++i) {
    if( array_contains(presenceList  , Students[i] )) {
      mySerial.println(presenceList[i]);
    }
  }
  mySerial.println("=============");
  mySerial.println("Absents  Users:");
  for(int i=0; i<sizeof(Students)/sizeof(String); ++i) {
    if(!array_contains(presenceList, Students[i])) {
      mySerial.println(Students[i]);
    }
  }
}
