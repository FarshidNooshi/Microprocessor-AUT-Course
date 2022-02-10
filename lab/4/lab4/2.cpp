#include <Arduino.h>
#include <Servo.h>
#include <Keypad.h>

#define ServoPin 2
#define beginAngle 0
#define endAngle 360
#define rowNum  4
#define colsNum  3

char keys[rowNum][colsNum] = {
    {'7', '8', '9'},
    {'4', '5', '6'},
    {'1', '2', '3'},
    {'#', '0', '*'}};

byte rowPins[rowNum] = {22, 23, 24, 25}; // connect to the row pinouts of the keypad
byte colPins[colsNum] = {26, 27, 28};     // connect to the column pinouts of the keypad

String inputStr = "";
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, rowNum, colsNum);

Servo myServo;  // create servo object to control a servo

int realToServoAngle(int angle);

void setup() {
  myServo.attach(ServoPin, 544, 2400);
  Serial2.begin(9600);
}

void loop() {
    char key = keypad.getKey();

    if (key != NO_KEY){
        Serial2.print(key);
        
        if (key == '#' || key == '*'){
            int degree = inputStr.toInt();
            
            if (degree < beginAngle || degree > endAngle){
                Serial2.println("Invalid Input");
            }
            else{
                myServo.write(realToServoAngle(degree));
            }
            
            inputStr = "";
        }
        else{
            inputStr += key;
        }
    }
}

int realToServoAngle(int angle) {
  return map(angle, beginAngle, endAngle, 0, 180);
}
