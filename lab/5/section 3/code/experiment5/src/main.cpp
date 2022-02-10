#include <Arduino.h>

const int active_pin = 53;

void setup()
{
  pinMode(active_pin, OUTPUT);
}

void loop()
{
  digitalWrite(active_pin, LOW);
  delay(1000);
  digitalWrite(active_pin, HIGH);
  delay(1000);
}