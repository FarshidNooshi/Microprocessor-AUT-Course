#include <Arduino.h>
#include <Servo.h>
#include <Keypad.h>

#define ServoPin 2
#define beginValue 0
#define endValue 1023

String inputStr = "";

Servo myServo;  // create servo object to control a servo

int realToServoAngle(int angle);

void setup() {
  myServo.attach(ServoPin, 544, 2400);
  Serial2.begin(9600);
}

void loop() {
    int pVal = analogRead(A0);
    int mappedPV = realToServoAngle(pVal);
    
    myServo.write(mappedPV);

    Serial2.print("Set New Degree To: ");
    Serial2.println(mappedPV);
    
    delay(1000);
}

int realToServoAngle(int angle) {
  return map(angle, beginValue, endValue, 0, 180);
}
