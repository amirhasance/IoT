#include <Servo.h>
#include <Keypad.h>

const int ROW_NUM = 4;
const int COLUMN_NUM = 3;
char keys[ROW_NUM][COLUMN_NUM] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};
byte pin_rows[ROW_NUM] = { 7 , 6 , 5 , 4 };
byte pin_column[COLUMN_NUM] = {10 , 9 , 8 };
Keypad keypad = Keypad( makeKeymap(keys), pin_rows, pin_column, ROW_NUM, COLUMN_NUM );

Servo servo1;
Servo servo2;
Servo servo3;

int Servo1_pin = 11;
int Servo2_pin = 12;
int Servo3_pin = 13;
int Moisture1_pin = A1;
int Moisture2_pin = A2;
int Moisture3_pin = A3;
int Sounder_pin = 2;
int Flame_pin = 3;


int selected_row;
bool Flame = false;
int degree;
String UserInput;



bool Servo1_is_open = false;
bool Servo2_is_open = false;
bool Servo3_is_open = false;

void setup() {
  
  Serial.begin(9600);

  pinMode(Moisture1_pin, INPUT);
  pinMode(Moisture2_pin, INPUT);
  pinMode(Moisture3_pin, INPUT);

  pinMode(Flame_pin, INPUT);
  
  pinMode(Sounder_pin, OUTPUT);


  servo1.attach(Servo1_pin);
  servo2.attach(Servo2_pin);
  servo3.attach(Servo3_pin);
  servo1.write(0);
  servo2.write(0);
  servo3.write(0);




}

void loop() {
  int soil1 , soil2 , soil3 ;
  
  get_keypad();
  flame_discovery();
  soil1 = map(analogRead(Moisture1_pin), 0, 1023, 0, 180);
  soil2 = map(analogRead(Moisture2_pin), 0, 1023, 0, 180);
  soil3 = map(analogRead(Moisture3_pin), 0, 1023, 0, 180);

  if(!Flame) {
    if(!Servo1_is_open) {
      servo1.write(soil1);
    }
    if(!Servo2_is_open) {
      servo2.write(soil2);
    }
    if(!Servo3_is_open) {
      servo3.write(soil3);
    }
  }
}

void printOnLcd(String Message) {
  Serial.println(Message);
}

void get_keypad() {
  Serial.println(UserInput);
  char inputKey = keypad.getKey();
  if(inputKey == '*') {
    selected_row = UserInput.toInt();
    UserInput = "";
  } else if(inputKey == '#') {
    degree = UserInput.toInt();
    if(selected_row == 1) {
      servo1.write(degree);
      Servo1_is_open = true;
    } else if(selected_row == 2) {
      servo2.write(degree);
      Servo2_is_open = true;
    } else {
      servo3.write(degree);
      Servo3_is_open = true;
    }
    UserInput = "";
  } else {
    UserInput += String(inputKey);
  }
}

void flame_discovery() {
  if(digitalRead(Flame_pin) == HIGH) {
    Flame = true;
    tone(Sounder_pin, 100);
    servo1.write(180);
    servo2.write(180);
    servo3.write(180);
    delay(2000);
    noTone(Sounder_pin);
    servo1.write(0);
    servo2.write(0);
    servo3.write(0);
    Flame = false;
  }
}
