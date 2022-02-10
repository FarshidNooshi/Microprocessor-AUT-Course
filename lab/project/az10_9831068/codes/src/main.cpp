#include <SPI.h>
#include <Arduino.h>
#include <Keypad.h>
#include <Wire.h>
#include <LiquidCrystal.h>
#include <Servo.h>
#include <math.h>

#define LED_NUM 3
#define PASSWORD "1234567"
#define DELAY 200
#define PRODUCTS_NUM 10
#define SERVO_NUM 11 // one of them for opening the door, others for delivering each product

#define MISO 3 // shift clock          is PB3
#define MOSI 2 // Master In Slave Out  is PB2
#define SCK 1  // Master Out Slave In  is PB1
#define SS 0   // Slave Select         is PB0

#define SLAVE_SELECT 53
#define uint_64 unsigned long long

char buf[256] = {'\0'}, reader[256] = {'\0'}, wr_buf[256] = {'o', 'k', '\0'}; // buffer for reading data from master
volatile int pos = 0;                                                         // position to add input to buffer

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
void spi_begin_slave();
void move_buffer_data();

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
  spi_begin_slave();
}

String inputStr = "";
bool inSetupMode = false;
bool isPrinted = false;
void loop()
{
  if (reader[0] == 'F') // it's emergency
  {
    inputStr = "";
    if (isPrinted == false) // checking if lcd has emergency string on itself or not, if don't we'll add it
    {
      lcd.clear();
      lcd.println("emergency");
      isPrinted = true;
    }
    turnOffLEDs(); // turning of LEDs and servos
    for (int i = 0; i < SERVO_NUM; i++)
    {
      turnOffServo(i);
    }
    return;
  }
  else if (isPrinted) // exiting from emergency situation
  {
    lcd.clear();
    turnOnLEDs();
    isPrinted = false;
  }
  char key = keypad.getKey();

  if (key != NO_KEY)
  {
    if (inSetupMode) // return to normal mode
    {
      for (int i = 0; i < SERVO_NUM; i++)
      {
        turnOnServo(i);
      }
      turnOnLEDs();

      inSetupMode = false;
      lcd.clear();
    }
    else if (key == '#') // clear lcd
    {
      inputStr = "";
      delay(DELAY);
      lcd.clear();
    }
    else if (key == '*')
    {
      lcd.clear();
      int inputInt = inputStr.toInt();

      if (inputStr.equals(PASSWORD)) // if entered string matches password go to setup mode
      {
        inSetupMode = true;
        lcd.print("Setup Mode");
        turnOffLEDs();
        for (int i = 0; i < SERVO_NUM; i++)
        {
          turnOffServo(i);
        }
      }
      else if (inputInt < PRODUCTS_NUM && inputInt > 0) // if entered string doesn't match password and is less than products number, deliver it.
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

void spi_begin_slave() // starting slave with handling appropriate bits for slave
{
  pinMode(MISO, OUTPUT);
  DDRB &= ~(1 << MOSI) & ~(1 << SCK) & ~(1 << SS); // 1 -> output, 0 -> input, deactivating MOSI, SCK, SS
  DDRB |= (1 << MISO);                             // activating MISO
  SPCR = (1 << SPE) | (1 << SPIE);                 // activating slave, spi
}

void move_buffer_data() // this method will move readden input from buffer to reader array
{
  memset(reader, 0, sizeof(reader));
  sprintf(reader, "%s", buf);
  memset(buf, 0, sizeof(buf));
  pos = 0;
}

ISR(SPI_STC_vect)
{
  if (digitalRead(SLAVE_SELECT) == LOW)
  {
    buf[pos] = SPDR; // data received from master is stored in SPDR
    pos++;

    if (pos < sizeof(buf))
    {
      if (buf[pos - 1] == '\n')
      {
        move_buffer_data();
      }
    }
    else
    {
      move_buffer_data();
    }

    SPDR = wr_buf[pos - 1];
    while (!(SPSR & (1 << SPIF))) // ta zamani ke khali nashode write kone, status register 1 -> rikhte, 0 -> narikhte
      ;
  }
}