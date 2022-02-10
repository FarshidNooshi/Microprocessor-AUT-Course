#include <SPI.h>
#include <Wire.h>
#include <Keypad.h>

#define ROWS 4
#define COLS 3

#define uint_64 unsigned long long

#define LM35 A0 // sensor
#define LDR A1

#define NSS_1 48 // slave select
#define NSS_2 49

#define MISO 3 // shift clock          is PB3
#define MOSI 2 // Master In Slave Out  is PB2
#define SCK 1  // Master Out Slave In  is PB1

char keys[ROWS][COLS] = {
    {'1', '2', '3'},
    {'4', '5', '6'},
    {'7', '8', '9'},
    {'C', '0', '='}};

byte rowPins[ROWS] = {40, 41, 42, 43};
byte colPins[COLS] = {24, 23, 22};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

String hold = "";
byte state = 0;
bool showMessage = true;
bool enableKeypad = true;

void reset()
{
    state = 0;
    hold = "";
    showMessage = enableKeypad = true;
    Serial.println();
}

void handleKeypad(KeypadEvent key)
{
    if (key == 'C')
    {
        reset();
        return;
    }

    if (!enableKeypad)
    {
        return;
    }

    if ('0' <= key && key <= '9')
    {
        Serial.print(key);
        hold += String(key - 48);
        return;
    }

    if (state == 0 && key == '=')
    {
        Serial.println();
        state = hold.toInt();
        showMessage = true;
        hold = "";
        return;
    }

    if (state != 0)
    {
        Serial.print(key);
    }
}

void key_event(KeypadEvent key)
{
    if (keypad.getState() == PRESSED)
    {
        handleKeypad(key);
    }
}

String transfer_data(String data, int slave_SS)
{
    String output = "";
    digitalWrite(slave_SS, LOW);
    for (int i = 0; i < data.length(); i++)
    {
        output += String(char(SPI.transfer(data[i])));
    }
    Serial.print("Master Send ");
    Serial.println(data);
    digitalWrite(slave_SS, HIGH);

    return output.substring(1);
}

void state_1()
{
    delay(200);
    String output = transfer_data("Farshid Nooshi 9831068\n", NSS_1);
    Serial.print(" , Received: ");
    Serial.println(output);
}

void state_2()
{
    delay(200);
    String output = transfer_data("Farshid Nooshi\n", NSS_2);
    output = "Hi \"" + output + "\"\n";
    delay(200);
    transfer_data(output, NSS_1);
}

void state_3()
{
    delay(300);
    int lm35 = analogRead(LM35);
    int ldr = analogRead(LDR);
    String output_1 = "Temperature: " + String(map(lm35, 0, 1023, 0, 40)) + "C\n";
    String output_2 = "Illuminance: " + String(map(ldr, 0, 1023, 50, 250)) + "CD\n";
    transfer_data(output_1, NSS_1);
    transfer_data(output_2, NSS_2);
}

void setup()
{

    SPI.setClockDivider(SPI_CLOCK_DIV2);
    SPI.begin();
    Serial.begin(9600);
    Serial.println("SPI MASTER:");

    pinMode(NSS_1, OUTPUT);
    pinMode(NSS_2, OUTPUT);
    pinMode(SS, OUTPUT);
    digitalWrite(NSS_1, HIGH);
    digitalWrite(NSS_2, HIGH);
    digitalWrite(SS, HIGH);

    keypad.addEventListener(key_event);
    keypad.setHoldTime(50);
}

void loop()
{
    char key = keypad.getKey();

    switch (state)
    {
    case 0:
        if (showMessage)
        {
            Serial.println("question number:");
            showMessage = false;
        }
        break;
    case 1:
        state_1();
        break;
    case 2:
        state_2();
        break;
    case 3:
        state_3();
        break;
    }
}