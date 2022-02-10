#include <Arduino.h>
#include <Wire.h>
#include <Keypad.h>
#include <LiquidCrystal.h>
#define TURN 2

const byte ROWS = 4; // four rows
const byte COLS = 4; // three columns
char keys[ROWS][COLS] = {
    {'7', '8', '9', '/'},
    {'4', '5', '6', 'x'},
    {'1', '2', '3', '-'},
    {'*', '0', '#', '+'}};
byte rowPins[ROWS] = {13, 12, 11, 10}; // connect to the row pinouts of the keypad
byte colPins[COLS] = {9, 8, 7, 6};     // connect to the column pinouts of the keypad
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

int board[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0}; // 0 means empty

LiquidCrystal lcd(25, 26, 27, 28, 29, 30, 31, 32, 33, 34);

int checkFinish();
bool isMyTurn(int turn_id);
void printBoard();
char getChar(int n);

void setup()
{
  Serial.begin(9600);
  lcd.begin(16, 4);
  printBoard();
}

void loop()
{
  int status = checkFinish();
  if (status == 0)
  {
    if (isMyTurn(TURN))
    {
      char key = keypad.getKey();
      if (key != NO_KEY)
      {
        int keyInt = key - 49;
        if (board[keyInt] == 0)
        {
          Serial.print(key);
          board[keyInt] = TURN;
          printBoard();
        }
      }
    }
    else if (Serial.available())
    {
      char data = Serial.read();
      int dataInt = data - 49;
      if (TURN == 1)
        board[dataInt] = 2;
      else
        board[dataInt] = 1;
      printBoard();
    }
  }
  else
  {
    if (status == 1)
    {
      lcd.clear();
      lcd.println("PLAYER 2 WON");
      delay(5000);
    }
    else if (status == 2)
    {
      lcd.clear();
      lcd.println("PLAYER 1 WON");
      delay(5000);
    }
    else
    {
      lcd.clear(); // no winner
      lcd.println("NO WINNER");
      delay(5000);
    }
  }
}

bool isMyTurn(int turn_id)
{
  int counter = 0;
  for (int i = 0; i < 9; i++)
  {
    if (board[i] != 0)
    {
      counter++;
    }
  }
  if (turn_id == 1)
  {
    return (counter % 2) == 0;
  }
  else
  {
    return (counter % 2) == 1;
  }
}

int checkFinish()
{
  if (board[0] != 0 && board[0] == board[1] && board[1] == board[2])
  {
    if (board[0] == 1)
    {
      return 1;
    }
    return 2;
  }

  else if (board[3] != 0 && board[3] == board[4] && board[4] == board[5])
  {
    if (board[3] == 1)
    {
      return 1;
    }
    return 2;
  }

  else if (board[6] != 0 && board[6] == board[7] && board[7] == board[8])
  {
    if (board[6] == 1)
    {
      return 1;
    }
    return 2;
  }

  else if (board[0] != 0 && board[0] == board[3] && board[3] == board[6])
  {
    if (board[0] == 1)
    {
      return 1;
    }
    return 2;
  }

  else if (board[1] != 0 && board[1] == board[4] && board[4] == board[7])
  {
    if (board[1] == 1)
    {
      return 1;
    }
    return 2;
  }

  else if (board[2] != 0 && board[2] == board[5] && board[5] == board[8])
  {
    if (board[2] == 1)
    {
      return 1;
    }
    return 2;
  }

  else if (board[0] != 0 && board[0] == board[4] && board[4] == board[8])
  {
    if (board[1] == 1)
    {
      return 1;
    }
    return 2;
  }

  else if (board[2] != 0 && board[2] == board[4] && board[4] == board[6])
  {
    if (board[3] == 1)
    {
      return 1;
    }
    return 2;
  }
  int counter = 0;
  for (int i = 0; i < 9; i++)
  {
    if (board[i] != 0)
    {
      counter++;
    }
  }
  if (counter == 9)
    return -1;
  else
    return 0;
}

void printBoard()
{
  lcd.clear();

  lcd.setCursor(6, 0);
  lcd.print(getChar(board[6]));
  lcd.print(" ");
  lcd.print(getChar(board[7]));
  lcd.print(" ");
  lcd.print(getChar(board[8]));
  lcd.print(" ");

  lcd.setCursor(6, 1);
  lcd.print(getChar(board[3]));
  lcd.print(" ");
  lcd.print(getChar(board[4]));
  lcd.print(" ");
  lcd.print(getChar(board[5]));
  lcd.print(" ");

  lcd.setCursor(6, 2);
  lcd.print(getChar(board[0]));
  lcd.print(" ");
  lcd.print(getChar(board[1]));
  lcd.print(" ");
  lcd.print(getChar(board[2]));
  lcd.print(" ");
}

char getChar(int n)
{
  if (n == 1)
  {
    return 'X';
  }
  else if (n == 2)
  {
    return 'O';
  }
  else
    return '#';
}