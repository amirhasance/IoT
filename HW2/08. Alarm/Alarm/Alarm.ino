#include <SoftwareSerial.h>
#include <Keypad.h>

SoftwareSerial terminal1(53, 52);

const int ROW_NUM = 4;
const int COLUMN_NUM = 3;
char keys[ROW_NUM][COLUMN_NUM] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};
byte pin_rows[ROW_NUM] = {21 , 20  , 19 , 18};
byte pin_column[COLUMN_NUM] = {17 , 16 ,15};
Keypad keypad = Keypad( makeKeymap(keys), pin_rows, pin_column, ROW_NUM, COLUMN_NUM );

String Message;
String Timer = "";
bool timer_is_set = false ;

void setup() {
  terminal1.begin(9600);
  Serial.begin(9600);
}

void loop() {
  char inputKey = keypad.getKey();
  if (inputKey) {
      if (timer_is_set){
          Serial.print(String(inputKey));
          terminal1.println(inputKey);
      }
      else{
          if (inputKey == '#'){   
              timer_is_set = true ;
              Serial.print(String(Message));
              terminal1.println("alarm set for : " + Message);
           }
          else {
                        
             Message += inputKey ; 
             terminal1.println(Message);
              
                  
            }
      }
    
  }
  

}
