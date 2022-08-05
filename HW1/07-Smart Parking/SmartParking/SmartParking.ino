#include <Keypad.h>
#include <LiquidCrystal.h>
#include <Keyboard.h>
#include <Servo.h>

const int ROW_NUM = 4;
const int COLUMN_NUM = 3;
char keys[ROW_NUM][COLUMN_NUM] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};
byte pin_rows[ROW_NUM] = {17, 18, 19, 20};
byte pin_column[COLUMN_NUM] = {14, 15, 16};
Keypad keypad = Keypad( makeKeymap(keys), pin_rows, pin_column, ROW_NUM, COLUMN_NUM );

Servo servo;
int SOUNDER_PORT = 13;


#define RS_PIN 33
#define EN_PIN 34
#define D4_PIN 37
#define D5_PIN 38
#define D6_PIN 39
#define D7_PIN 40
LiquidCrystal lcd(RS_PIN, EN_PIN,D4_PIN, D5_PIN,D6_PIN, D7_PIN );

int LED_A1 = 1;
int LED_A2 = 2;
int LED_A3 = 3;
int LED_B1 = 4;
int LED_B2 = 5;
int LED_B3 = 6;
int LED_C1 = 7;
int LED_C2 = 8;
int LED_C3 = 9;

int LED_LIST [9] = {LED_A1 ,LED_A2 , LED_A3 , LED_B1 ,LED_B2 ,LED_B3 ,LED_C1 , LED_C2 , LED_C3  };

int PIR_A1 = 22;
int PIR_A2 = 23;
int PIR_A3 = 24;
int PIR_B1 = 25;
int PIR_B2 = 26;
int PIR_B3 = 27;
int PIR_C1 = 28;
int PIR_C2 = 29;
int PIR_C3 = 30;

int PIR_LIST [9] = {PIR_A1 ,PIR_A2 , PIR_A3 ,PIR_B1 , PIR_B2 , PIR_B3 , PIR_C1 , PIR_C2  , PIR_C3};

String usernames[9] = {"111", "222", "333", "444", "555", "666", "777", "888", "999"};
String passwords[9] = {"111", "222", "333", "444", "555", "666", "777", "888", "999"};

String userType;
bool entrance = false;
bool isExit = false;
bool enterId = false;
bool enterPass = false;
int parkingSpot[9] = {1, 1, 1, 1, 1, 1, 1, 1, 1};
int parkingTime[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};

void setup() {
  lcd.begin(16, 4);

  servo.attach(SOUNDER_PORT);
  servo.write(0);

  for(int i=0 ; i< 9 ; i++){
     pinMode(PIR_LIST[i], INPUT);
  }
  for(int i=0; i<9 ; i++){
    pinMode(LED_LIST[i], OUTPUT);
  }
}



void loop() {
  checkLeds();
  printSpotsInfo();
  readKeypad();
}

void checkLeds() {
  for(int i=0 ; i < 9 ; i++)
  {
      digitalWrite(LED_LIST[i] , digitalRead(PIR_LIST[i]));
      if(digitalRead(PIR_LIST[i]) == HIGH) {
          parkingSpot[i] = 0;} 
      else {
          parkingSpot[i] = 1;
  }
 }
}

void printSpotsInfo() {
  int fa = 0;
  int fb = 0;
  int fc = 0;
  int empty_capacity = 0;
  for(int i=0; i<9; ++i) {
    if(parkingSpot[i]==1) {
      ++empty_capacity;
    }
    if( i< 3 ){
      if(parkingSpot[i]==1) {
        ++fa;
      }
    }else {
         if(i < 6 ){
      if(parkingSpot[i]==1) {
        ++fb;
      }
    }else {
       if(i < 9 ){
      if(parkingSpot[i]==1) {
        ++fc;
      }}}
   
    }
  }
  printOnLcd(0, "FA: " + String(fa) + " FB: " + String(fb));
  printOnLcd(1, "FC: " + String(fc) + " Empty: " + String(empty_capacity));
}

void printOnLcd(int lineNo, String Message) {
  lcd.setCursor(0, lineNo);
  lcd.print(Message);
}

void clearOnLcd(int lineNo) {
  lcd.setCursor(0, lineNo);
  lcd.clear();
}

void readKeypad() {
  char inputKey = keypad.getKey();
  printOnLcd(2, String(inputKey));
  if(userType == "1*") {
    entrance = true;
    userType = "";
  }
  if(userType == "2*") {
    isExit = true;
    userType = "";
  }
  if(!entrance && !isExit) {
    userType += String(inputKey);
  }
  if(entrance) {
    printOnLcd(3, userType);
    if(inputKey == '*') {
      if(existInArray(usernames, userType, 0) == 1) {
        userType = "";
        clearOnLcd(3);
      } else if(existInArray(usernames, userType, 0) == 0) {
        clearOnLcd(3);
        userType = "";
        printOnLcd(3, "Username is not exists!");
        entrance = false;
        clearOnLcd(3);  
      } else {
        userType = "";
        clearOnLcd(3);
      }
    } else if(inputKey == '#') {
      if(existInArray(passwords, userType, 1) == 1) {
        entrance = false;
        userType = "";
        servo.write(180);
        delay(1000);
        servo.write(0);
        clearOnLcd(3);
      } else if(existInArray(passwords, userType, 1) == 0) {
        userType = "";
        printOnLcd(3, "Password is not correct!");
        entrance = false;
        clearOnLcd(3);  
      } else {
          userType = "";
          clearOnLcd(3);
        }
    } else {
      userType += String(inputKey);
    }
  }
  if(isExit) {
    printOnLcd(3, userType);
    if(inputKey == '*') {
      if(existInArray(usernames, userType, 0) == 1) {
        userType = "";
        clearOnLcd(3);
      } else if(existInArray(usernames, userType, 0) == 0) {
        clearOnLcd(3);
        userType = "";
        printOnLcd(3, "Username is not exists!");
        entrance = false;
        clearOnLcd(3);  
      } else {
          userType = "";
          clearOnLcd(3);
        }
    } else if(inputKey == '#') {
      if(existInArray(passwords, userType, 2) == 1) {
        isExit = false; 
        userType = "";
        servo.write(180);
        delay(1000);
        servo.write(0);
        clearOnLcd(3);
      } else if(existInArray(usernames, userType, 0) == 0) {
        userType = "";
        printOnLcd(3, "Password is not correct!");
        entrance = false;
        clearOnLcd(3);  
      } else {
          userType = "";
          clearOnLcd(3);
        }
    } else {
      userType += String(inputKey);
    }
  }
}

int existInArray(String *input, String element, int type) {
  for(int i=0; i<9; ++i) {
    if(input[i] == element) {
      if(type == 1) {
        if(parkingSpot[i] == 1) {
          parkingSpot[i] = 0;
          parkingTime[i] = millis();
          reserveSpot(0, i);
        } else {
          printOnLcd(3, "No Space is available");
          return 2;
        }
      }
      if(type == 2) {
        clearOnLcd(3);
        parkingSpot[i] = 1;
        int price = (abs((millis() - parkingTime[i])/1000) * 100);
        printOnLcd(2, String((millis() - parkingTime[i])/1000));
        printOnLcd(3, "Price: " + String(price));
        reserveSpot(1, i);
      }
      return 1;
    }  
  }
  return 0;
}

bool enterCar() {
  for(int i=0; i<9; ++i) {
    if(parkingSpot[i] == 1) {
      parkingSpot[i] = 0;
      parkingTime[i] = millis();
    }  
  }
}

void reserveSpot(int type, int index) {
    if(type == 0) {
        digitalWrite(LED_LIST[index] , HIGH);}
    else {
        digitalWrite(LED_LIST[index] , LOW);
      }
 }
