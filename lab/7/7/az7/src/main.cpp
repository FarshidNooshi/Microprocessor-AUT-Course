#include <Arduino.h>
#include <Keypad.h>
#include <LiquidCrystal.h>
#include <Wire.h>

#define LEDY11 22
#define LEDY12 23
#define LEDY21 24
#define LEDY22 25

#define DEV_ADDR 0b1010000
#define MODE_ADDR 0xAA
#define M1T_ADDR 0xBB
#define M2T_ADDR 0xCC
#define M3T_ADDR 0xDD
#define M4T_ADDR 0xEE

#define PREWASH 1
#define CLEANER 2
#define WATERING 3
#define DRYING 4

// creat lcd
const int rs = 13, en = 12, d4 = 11, d5 = 10, d6 = 9, d7 = 8;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// create 4*3 keypad
const byte ROWS = 4; // number of rows
const byte COLS = 3; // number of columns
char keys[ROWS][COLS] = {{'1', '2', '3'},
                         {'4', '5', '6'},
                         {'7', '8', '9'},
                         {'*', '0', '#'}};
byte rowPins[ROWS] = {50, 51, 52, 53};
byte colPins[COLS] = {49, 48, 47};
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// usefull variables definition
uint8_t modeNumber, changeTimeModeNumber;
int m1t = 2, m2t = 2, m3t = 2, m4t = 2;
boolean starting = true, chMod = false, initState = true, TimeMod = false, ChTimeMode = false;

// usefull functions definitions
String modeToString(uint8_t modeNum);
void writeToEEPROM(uint8_t data, uint8_t address);
uint8_t readFromEEPROM(uint8_t address);
void startWashing();
void goToControlPanel();
void clear();

void setup()
{
  pinMode(LEDY11, OUTPUT);
  pinMode(LEDY12, OUTPUT);
  pinMode(LEDY21, OUTPUT);
  pinMode(LEDY22, OUTPUT);

  lcd.begin(16, 2);
  lcd.clear();
  lcd.setCursor(0, 0);

  Wire.begin(); // join i2c bus
}

void loop()
{
  if (starting)
  {
    modeNumber = readFromEEPROM(MODE_ADDR);
    m1t = readFromEEPROM(M1T_ADDR);
    m2t = readFromEEPROM(M2T_ADDR);
    m3t = readFromEEPROM(M3T_ADDR);
    m4t = readFromEEPROM(M4T_ADDR);

    // changeTimes(modeNumber);
    goToControlPanel();
    starting = false;
  }

  char keyPressed = keypad.getKey();
  if (keyPressed)
  {

    if (keyPressed == '*' && !initState) // set initial state
    {
      clear();
      goToControlPanel();
      digitalWrite(LEDY11, LOW);
      digitalWrite(LEDY12, LOW);
      digitalWrite(LEDY21, LOW);
      digitalWrite(LEDY22, LOW);
      initState = true;
    }

    else if (keyPressed == '*') // start washing in initted mode
    {
      startWashing();
      initState = false;
    }

    // clear lcd and reset everything
    else if (keyPressed == '#' && initState)
    {
      clear();
    }

    // user has typed 9
    else if (keyPressed == '9' && initState && !TimeMod)
    {
      clear();
      lcd.print("Type 1-4 : ");
      TimeMod = true;
    }

    // user has typed 1-4 to change time
    else if ((keyPressed == '1' || keyPressed == '2' || keyPressed == '3' || keyPressed == '4') && TimeMod && initState && !ChTimeMode)
    {
      lcd.print(keyPressed);
      lcd.setCursor(0, 1);
      changeTimeModeNumber = keyPressed - '0';
      ChTimeMode = true;
      lcd.print("Type seconds : ");
    }

    else if (isDigit(keyPressed) && TimeMod && ChTimeMode && initState)
    {
      lcd.print(keyPressed);
      if (changeTimeModeNumber == 1)
      {
        m1t = keyPressed - '0';
        writeToEEPROM(m1t, M1T_ADDR);
      }
      else if (changeTimeModeNumber == 2)
      {
        m2t = keyPressed - '0';
        writeToEEPROM(m2t, M2T_ADDR);
      }
      else if (changeTimeModeNumber == 3)
      {
        m3t = keyPressed - '0';
        writeToEEPROM(m3t, M3T_ADDR);
      }
      else if (changeTimeModeNumber == 4)
      {
        m4t = keyPressed - '0';
        writeToEEPROM(m4t, M4T_ADDR);
      }
      TimeMod = false;
      ChTimeMode = false;
      delay(200);
      clear();
      goToControlPanel();
    }

    // user has typed 5
    else if (keyPressed == '5' && initState)
    {
      clear();
      lcd.print("Type 1-4 : ");
      chMod = true;
    }

    // user has typed 1-4 to change default mode
    else if ((keyPressed == '1' || keyPressed == '2' || keyPressed == '3' || keyPressed == '4') && chMod && initState)
    {
      lcd.print(keyPressed);
      modeNumber = keyPressed - '0';
      writeToEEPROM(modeNumber, MODE_ADDR);
      // changeTimes(modeNumber);
      chMod = false;
      delay(200);
      clear();
      goToControlPanel();
    }
  }
}

String modeToString(uint8_t modeNum)
{
  if (modeNum == 2)
    return "Cleaning";
  if (modeNum == 3)
    return "Watering";
  if (modeNum == 4)
    return "Drying";
  return "Prewashing";
}

void writeToEEPROM(uint8_t data, uint8_t address)
{
  Wire.beginTransmission(DEV_ADDR);
  Wire.write(address);
  Wire.write(data);
  Wire.endTransmission();
  delay(1000);
}

uint8_t readFromEEPROM(uint8_t address)
{
  // Dummy Write
  Wire.beginTransmission(DEV_ADDR);
  Wire.write(address);
  Wire.endTransmission();

  Wire.requestFrom(DEV_ADDR, 1);
  uint8_t v = Wire.read();
  return v;
}

void startWashing()
{

  uint8_t start_mode = readFromEEPROM(MODE_ADDR);
  if (start_mode == 1)
  {
    digitalWrite(LEDY11, HIGH);
    for (int i = 0; i < m1t; i++)
    {
      clear();
      lcd.print("Prewashing ...");
      lcd.setCursor(0, 1);
      lcd.print(m1t - i);
      lcd.print(" seconds");
      delay(500);
    }
    start_mode++;
    writeToEEPROM(start_mode, MODE_ADDR); // save it for when the machine has turned off
    digitalWrite(LEDY11, LOW);
  }
  if (start_mode == 2)
  {
    digitalWrite(LEDY12, HIGH);
    for (int i = 0; i < m2t; i++)
    {
      clear();
      lcd.print("Cleaning ...");
      lcd.setCursor(0, 1);
      lcd.print(m2t - i);
      lcd.print(" seconds");
      delay(500);
    }
    start_mode++;
    writeToEEPROM(start_mode, MODE_ADDR); // save it for when the machine has turned off
    digitalWrite(LEDY12, LOW);
  }
  if (start_mode == 3)
  {
    digitalWrite(LEDY21, HIGH);
    for (int i = 0; i < m3t; i++)
    {
      clear();
      lcd.print("Watering ...");
      lcd.setCursor(0, 1);
      lcd.print(m3t - i);
      lcd.print(" seconds");
      delay(500);
    }
    start_mode++;
    writeToEEPROM(start_mode, MODE_ADDR); // save it for when the machine has turned off
    digitalWrite(LEDY21, LOW);
  }
  if (start_mode == 4)
  {
    digitalWrite(LEDY22, HIGH);
    for (int i = 0; i < m4t; i++)
    {
      clear();
      lcd.print("Drying ...");
      lcd.setCursor(0, 1);
      lcd.print(m4t - i);
      lcd.print(" seconds");
      delay(500);
    }
    writeToEEPROM(modeNumber, MODE_ADDR); // everything was ok, so start from modeNumber in later iterations
    digitalWrite(LEDY22, LOW);
  }

  digitalWrite(LEDY11, HIGH);
  digitalWrite(LEDY12, HIGH);
  digitalWrite(LEDY21, HIGH);
  digitalWrite(LEDY22, HIGH);
  clear();
  lcd.print("Done");
}

void clear()
{
  lcd.clear();
  lcd.setCursor(0, 0);
}

void goToControlPanel()
{
  lcd.print("Mode: ");
  lcd.print(modeToString(modeNumber));
  lcd.setCursor(0, 1);
  lcd.print("5 to change mode");
}