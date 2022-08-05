#include <SoftwareSerial.h>
#include <Keypad.h>

SoftwareSerial terminal1(53, 52);
//SoftwareSerial xbee(1, 0);

const int ROW_NUM = 4;
const int COLUMN_NUM = 3;
char keys[ROW_NUM][COLUMN_NUM] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};
byte pin_rows[ROW_NUM] = {21 , 20 , 19 , 18};
byte pin_column[COLUMN_NUM] = {17 , 16 , 15 };
Keypad keypad = Keypad( makeKeymap(keys), pin_rows, pin_column, ROW_NUM, COLUMN_NUM );

String pass = "";
char entered_key = "";
void setup() {
  terminal1.begin(9600);
  Serial.begin(9600);
}

void loop() {
  f();
}
void f(){
  entered_key = keypad.getKey();
  if (entered_key) {
    if (entered_key == '#') {
      Serial.print(pass);
      pass = "";
    } else {
      pass += entered_key;
      terminal1.println(pass);
    }
  }
}
