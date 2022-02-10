#include <Keypad.h>

const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
const int leds[] = {0, 1, 2, 3, 4, 5, 6, 7, 8}, len_leds = sizeof(leds)/sizeof(*leds);
char keys[ROWS][COLS] = {
  {'7','8','9', '/'},
  {'4','5','6', '*'},
  {'1','2','3', '-'},
  {'C','0','=', '+'}
};
byte rowPins[ROWS] = {23, 25, 27, 29}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {31, 33, 35, 37}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

void setup(){
  Serial2.begin(9600);
  // put your setup code here, to run once:
  for(int i = 0; i < len_leds; i++){
    pinMode(leds[i], OUTPUT);
  }
  changeState(len_leds, LOW);
}
  
void loop(){
  char key = keypad.getKey();
  
  if (key){
    Serial2.println(key);
    if (key >= '0' && key <= '9') {
      int numberOfChanges = key - '0';
      
      changeState(numberOfChanges, HIGH);
    } else {
      changeState(len_leds, LOW);
    }
  }
}

void changeState(int numberOfChanges, int state) {
  for (int i = 0; i < numberOfChanges; i++) {
    digitalWrite(leds[len_leds - i - 1], state);
  }
}

