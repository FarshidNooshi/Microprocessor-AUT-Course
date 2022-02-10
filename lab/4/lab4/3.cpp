#include <Arduino.h>
#include <Servo.h>
#include <Keypad.h>

#define ServoPin 2
#define beginAngle -180
#define endAngle 180

String inputStr = "";
int curr_degree = 0;

Servo myServo; // create servo object to control a servo

int realToServoAngle(int angle);

void setup()
{
  myServo.attach(ServoPin, 544, 2400);
  Serial2.begin(9600);
}

void loop()
{

  while (Serial2.available() == 0)
    ;

  inputStr = Serial2.readString();
  int entered_degree = inputStr.toInt();

  curr_degree = -1 * entered_degree + curr_degree;
  if (curr_degree < beginAngle || curr_degree > endAngle)
  {
    curr_degree = curr_degree + entered_degree;
    Serial2.println("Invalid Input");
  }
  else
  {
    myServo.write(realToServoAngle(curr_degree));
    Serial2.print("Set New Degree To: ");
    Serial2.println(curr_degree);
  }
}

int realToServoAngle(int angle)
{
  return map(angle, beginAngle, endAngle, 0, 180);
}
