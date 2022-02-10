#include <SPI.h>
#include <Arduino.h>
#include <Keypad.h>
#include <Wire.h>
#include <LiquidCrystal.h>
#include <Servo.h>

#define uint_64 unsigned long long

#define LM35 A0 // sensor
#define LDR A1

#define SOUND_PIN 8 // for tone

#define LOW_TEMP 0   // lowest degree in simulation
#define HIGH_TEMP 40 // highest degree in simulation

#define NSS_1 48 // slave select
#define NSS_2 49

#define MISO 3 // shift clock          is PB3
#define MOSI 2 // Master In Slave Out  is PB2
#define SCK 1  // Master Out Slave In  is PB1

#define LED_NUM 1   // number of LEDs
#define DELAY 200   // delay unit
#define SERVO_NUM 3 // number of servos

const int LED_PINS[LED_NUM] = {36};
const int SERVO_PINS[SERVO_NUM] = {2, 3, 4};

String hold = "";
byte state = 0;
bool showMessage = true;
bool enableKeypad = true;

Servo Servos[SERVO_NUM];

void turnOnLEDs();
void turnOffLEDs();
void goSpecial();
void updateTemp();
void goOutOfSpecial();

String transfer_data(String data, int slave_SS) // this method transfers data via SPI protocol and returns the readden input from the selected slave
{
    String output = "";
    digitalWrite(slave_SS, LOW); // selecting slave
    for (int i = 0; i < data.length(); i++)
    {
        output += String(char(SPI.transfer(data[i]))); // transferring data and meanwhile reading from slave
    }
    Serial.print("Master Send "); // printing what we've sent to our slave
    Serial.println(data);
    digitalWrite(slave_SS, HIGH); // deselecting slave

    return output.substring(1);
}

void setup()
{
    SPI.setClockDivider(SPI_CLOCK_DIV2); // initiating clock divider for SPI
    SPI.begin();                         // begining SPI
    Serial.begin(9600);
    Serial.println("SPI MASTER:");

    pinMode(NSS_1, OUTPUT);    // initiating SPI
    pinMode(NSS_2, OUTPUT);    // setting the type of our NSS
    pinMode(SS, OUTPUT);       // slave select
    digitalWrite(NSS_1, HIGH); // initiating HIGH for NSS and SS
    digitalWrite(NSS_2, HIGH);
    digitalWrite(SS, HIGH);

    for (int i = 0; i < LED_NUM; i++)
    {
        pinMode(LED_PINS[i], OUTPUT);
    }

    for (int i = 0; i < SERVO_NUM; i++)
    {
        Servos[i].attach(SERVO_PINS[i], 544, 2400);
    }
    turnOffLEDs();
}

int temperature = 0; // [0, 40]
int score = 0;
int isSpecial = 0;
int counter = 0;

void loop()
{
    if (isSpecial)
    {
        tone(SOUND_PIN, counter * DELAY, DELAY * 10); // playing sound for alert
        counter++;
    }
    else
    {
        counter = 0;
        tone(SOUND_PIN, 0, DELAY); // finish the alert
    }
    delay(DELAY * 1.2);         // waiting a little
    Serial.print("score is: "); // printing result
    Serial.println(score);
    updateTemp();
    if (temperature >= 30) // it's emergency now!
    {
        goSpecial(); // go to emergency situation
        return;
    }
    if (isSpecial)
    {
        goOutOfSpecial(); // exit from emergency situation if we were
        return;
    }
    for (int i = 0; i < SERVO_NUM; i++)
    {
        Servos[i].write(map(temperature, LOW_TEMP, HIGH_TEMP, 0, 180)); // updating servos with relative values
    }
    if (temperature >= 15) // revarding ourselves
    {
        score++;
    }
}

void turnOnLEDs() // this method will turn an LED on
{
    for (int i = 0; i < LED_NUM; i++)
    {
        digitalWrite(LED_PINS[i], HIGH);
    }
}
void turnOffLEDs() // this method will turn an LED Off
{
    for (int i = 0; i < LED_NUM; i++)
    {
        digitalWrite(LED_PINS[i], LOW);
    }
}

int stat = 1;

void updateTemp() // updating the temperature with two possible states, one with LM35 and one with simulating by increasing or decreasing on unit per execution of this method
{
    // int lm35 = analogRead(LM35);
    // temperature = lm35;
    temperature += stat;
    if (temperature == HIGH_TEMP || temperature == LOW_TEMP)
        stat *= -1;
}

void goSpecial() // go to emergency situation and tell slaves to go to emergency too.
{
    isSpecial = 1;
    transfer_data("Failure\n", NSS_1);
    transfer_data("Failure\n", NSS_2);
    turnOnLEDs(); // turn on alert LED
    for (int i = 0; i < SERVO_NUM; i++)
    {
        Servos[i].write(180);
    }
}

void goOutOfSpecial() // get out of emergency situation and tell slaves to get out too
{
    isSpecial = 0;
    transfer_data("Safe\n", NSS_1);
    transfer_data("Safe\n", NSS_2);
    turnOffLEDs(); // turn off alert LED
    for (int i = 0; i < SERVO_NUM; i++)
    {
        Servos[i].write(map(temperature, LOW_TEMP, HIGH_TEMP, 0, 180));
    }
}