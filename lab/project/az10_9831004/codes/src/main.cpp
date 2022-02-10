#include <Arduino.h>
#include "pitches.h"
#define SOUND_PIN 8 // sounder pin

const int LED_PIN[] = {2, 3, 4}; // led pins
const int POT_PIN[] = {A0, A1, A2}; // potentiometer pins 
bool isPlaying[3] = {false, false, false}; // check if o or m or k should be played
int keyPressed[3] = {0, 0, 0};// used to track what user entered from left-side keyboard characters

void play(int index);
void tone_note(int sound_pin, int freq, int duration);
void stopPlaying(char c, int index);
void startPlaying(char c, int index);

void setup() // strting serial terminal
{
  Serial.begin(9600);
}

void loop()
{
  if (Serial.available() > 0)
  {
    char key = Serial.read();

    if (key == 'm') // check what sound the guitar had to play
    {
      if (isPlaying[2]) // if it pressed before, stop playing
        stopPlaying('m', 2);
      else
        startPlaying('m', 2);
    }
    else if (key == 'k')
    {
      if (isPlaying[1])
        stopPlaying('k', 1);
      else
        startPlaying('k', 1);
    }
    else if (key == 'o')
    {
      if (isPlaying[0])
        stopPlaying('o', 0);
      else
        startPlaying('o', 0);
    }
    else // if a left keyboard character is pressed
    {
      Serial.print(key);
      Serial.println(" Pressed");
      if (key == 'q')
        keyPressed[0] = Q_Key;
      else if (key == 'a')
        keyPressed[1] = A_Key;
      else if (key == 'z')
        keyPressed[2] = Z_Key;
      else if (key == 'w')
        keyPressed[0] = W_Key;
      else if (key == 's')
        keyPressed[1] = S_Key;
      else if (key == 'x')
        keyPressed[2] = X_Key;
      else if (key == 'e')
        keyPressed[0] = E_Key;
      else if (key == 'd')
        keyPressed[1] = D_Key;
      else if (key == 'c')
        keyPressed[2] = C_Key;
      else if (key == 'r')
        keyPressed[0] = R_Key;
      else if (key == 'f')
        keyPressed[1] = F_Key;
      else if (key == 'v')
        keyPressed[2] = V_Key;
      else if (key == 't')
        keyPressed[0] = T_Key;
      else if (key == 'g')
        keyPressed[1] = G_Key;
      else if (key == 'b')
        keyPressed[2] = B_Key;
      else if (key == 'y')
        keyPressed[0] = Y_Key;
      else if (key == 'h')
        keyPressed[1] = H_Key;
      else if (key == 'n')
        keyPressed[2] = N_Key;
      else
        Serial.println("Invalid Input");
    };
  }

  play(0); // play sound of each row
  play(1);
  play(2);
}

void play(int index) //calls tone_note if is playing is true
{
  if (isPlaying[index])
  {
    float scale = analogRead(POT_PIN[index]) / 512.0;
    int freq = (int)(MELODY[index][keyPressed[index]] * scale);
    tone_note(SOUND_PIN, freq, NOTES_DURATION);
  }
}

void tone_note(int sound_pin, int freq, int duration) // plays the sound
{
  tone(sound_pin, freq, duration);
  delay(int(duration * 1.1));
}

void stopPlaying(char c, int index) // stops playing sound and turns LED off
{
  Serial.print("STOP Playing: ");
  Serial.println(c);
  isPlaying[index] = false;
  digitalWrite(LED_PIN[index], LOW);
}

void startPlaying(char c, int index) // starts playing sound and turns LED on
{
  Serial.print("START Playing: ");
  Serial.println(c);
  isPlaying[index] = true;
  digitalWrite(LED_PIN[index], HIGH);
}
