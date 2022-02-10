#include <Arduino.h>
#include <Keypad.h>

#include "pitches.h"
#include "themes1.h"
#include "themes2.h"
#include "themes3.h"
#include "themes4.h"
#include "themes5.h"

const int POT_PIN = A0;
#define SOUND_PIN 8

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
float scale;

float *getNoteDuration(int selectedSound);
float *getMelody(int selectedSound);
int getMelodySize(int selectedSound);

void setup()
{
}

int selectedSound = 1;
void loop()
{
  char keyPressed = keypad.getKey();
  if (keyPressed)
  {
    if (keyPressed == '*' || keyPressed == '#' || keyPressed == '0') // the play the selected sound
    {
      float *melody = getMelody(selectedSound);
      float *noteDurations = getNoteDuration(selectedSound);
      for (int note = 0; note < getMelodySize(selectedSound); note++)
      {
        scale = analogRead(POT_PIN) / 512.0;
        int duration = 800.0 / (noteDurations[note]);
        tone(SOUND_PIN, (int)(melody[note] * scale), duration);
        delay((int)(1.2*duration));
      }
    }
    else
    {
      selectedSound = int(keyPressed - '0');
    }
  }
}

float *getNoteDuration(int selectedSound)
{
  switch (selectedSound)
  {
  case 1:
    return noteDurations1;
    break;
  case 2:
    return noteDurations2;
    break;
  case 3:
    return noteDurations3;
    break;
  case 4:
    return noteDurations4;
    break;
  case 5:
    return noteDurations5;
    break;

  default:
    return noteDurations1;
    break;
  }
}
float *getMelody(int selectedSound)
{
  switch (selectedSound)
  {
  case 1:
    return melody1;
    break;
  case 2:
    return melody2;
    break;
  case 3:
    return melody3;
    break;
  case 4:
    return melody4;
    break;
  case 5:
    return melody5;
    break;
  default:
    return melody5;
    break;
  }
}
int getMelodySize(int selectedSound)
{
  switch (selectedSound)
  {
  case 1:
    return sizeof(melody1) / sizeof(int);
    break;
  case 2:
    return sizeof(melody2) / sizeof(int);
    break;
  case 3:
    return sizeof(melody3) / sizeof(int);
    break;
  case 4:
    return sizeof(melody4) / sizeof(int);
    break;
  case 5:
    return sizeof(melody5) / sizeof(int);
    break;
  default:
    return sizeof(melody1) / sizeof(int);
    break;
  }
}