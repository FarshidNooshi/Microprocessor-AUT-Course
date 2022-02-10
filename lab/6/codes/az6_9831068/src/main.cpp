#include <Arduino.h>
#include <Keypad.h>
#include <Wire.h>
#include <LiquidCrystal.h>
#include <Servo.h>

#define LED_NUM 3
#define PASSWORD "1234567"
#define DELAY 200
#define PRODUCTS_NUM 10
#define SERVO_NUM 11 //one of them for opening the door, others for delivering each product

const int LED_PINS[LED_NUM] = {40, 41, 42};
const int SERVO_PINS[SERVO_NUM] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

const byte rowsNum = 4;
const byte colsNum = 3;
char keys[rowsNum][colsNum] = {
    {'1', '2', '3'},
    {'4', '5', '6'},
    {'7', '8', '9'},
    {'*', '0', '#'}};

byte rowPins[rowsNum] = {25, 26, 27, 28};
byte colPins[colsNum] = {24, 23, 22};
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, rowsNum, colsNum);

Servo Servos[SERVO_NUM];

const int rs = 30, en = 31, d[] = {32, 33, 34, 35, 36, 37, 38, 39};
LiquidCrystal lcd(rs, en, d[0], d[1], d[2], d[3], d[4], d[5], d[6], d[7]);

void turnOnLEDs();
void turnOffLEDs();
void turnOffServo(int servoNum);
void turnOnServo(int servoNum);

void setup()
{
  lcd.begin(16, 2);

  for (int i = 0; i < LED_NUM; i++)
  {
    pinMode(LED_PINS[i], OUTPUT);
  }

  for (int i = 0; i < SERVO_NUM; i++)
  {
    Servos[i].attach(SERVO_PINS[i], 544, 2400);
  }
  turnOnLEDs();
}

String inputStr = "";
bool inSetupMode = false;
void loop()
{
  char key = keypad.getKey();

  if (key != NO_KEY)
  {
    if (inSetupMode) //return to normal mode
    {
      for (int i = 0; i < SERVO_NUM; i++)
      {
        turnOnServo(i);
      }
      turnOnLEDs();

      inSetupMode = false;
      lcd.clear();
    }
    else if (key == '#') //clear lcd
    {
      inputStr = "";
      delay(DELAY);
      lcd.clear();
    }
    else if (key == '*')
    {
      lcd.clear();
      int inputInt = inputStr.toInt();

      if (inputStr.equals(PASSWORD)) //if entered string matches password go to setup mode
      {
        inSetupMode = true;
        lcd.print("Setup Mode");
        turnOffLEDs();
        for (int i = 0; i < SERVO_NUM; i++)
        {
          turnOffServo(i);
        }
      }
      else if (inputInt < PRODUCTS_NUM && inputInt > 0) //if entered string doesn't match password and is less than products number, deliver it.
      {
        lcd.print("Delivering " + inputStr);
        turnOffServo(inputInt);
        delay(10 * DELAY);
        turnOnServo(inputInt);
        lcd.clear();
      }
      else
      {
        lcd.print("Wrong INP");
        delay(DELAY * 4);
        lcd.clear();
      }

      inputStr = "";
    }
    else
    {
      lcd.print(key);
      inputStr = inputStr + key;
    }
  }
}

void turnOnLEDs()
{
  for (int i = 0; i < LED_NUM; i++)
  {
    digitalWrite(LED_PINS[i], HIGH);
  }
}
void turnOffLEDs()
{
  for (int i = 0; i < LED_NUM; i++)
  {
    digitalWrite(LED_PINS[i], LOW);
  }
}
void turnOffServo(int servoNum)
{
  Servos[servoNum].write(0);
}
void turnOnServo(int servoNum)
{
  Servos[servoNum].write(180);
}