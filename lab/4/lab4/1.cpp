#include <Arduino.h>
#include <Servo.h>

#define ServoPin 2
#define beginAngle -180
#define endAngle 180

Servo myServo;  // create servo object to control a servo

int switchTeam = 1;
int val = 0;    // variable to read the value from the analog pin

int realToServoAngle(int angle);

void setup() {
  myServo.attach(ServoPin, 544, 2400);
}

void loop() {
  val = (val + 1) % 180;
  myServo.write(realToServoAngle(90 - abs(90 - val)));
  delay(15);
}

int realToServoAngle(int angle) {
  return map(angle, beginAngle, endAngle, 0, 180);
}

