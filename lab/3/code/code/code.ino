#include <Vector.h>
#include <LiquidCrystal.h>
#include <Keypad.h>

#define ROWS  4
#define COLS  4

#define rows 2
#define cols 16

#define NAME "Farshid"
#define PASSWORD "9831068"

const int rs = 30, en = 31, d[] = {32, 33, 34, 35, 36, 37, 38, 39};
LiquidCrystal lcd(rs, en, d[0], d[1], d[2], d[3], d[4], d[5], d[6], d[7]);

int colIndex = 0, rowIndex = 0;

char keys[ROWS][COLS] = {
  {'7', '8', '9', '/'},
  {'4', '5', '6', '*'},
  {'1', '2', '3', '-'},
  {'C', '0', '=', '+'}
};

byte rowPins[ROWS] = {22, 23, 24, 25};
byte colPins[COLS] = {26, 27, 28, 29};

int state = 0;
String hold = "";
int first, second;
char operand = '!';

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

void setup() {
  // put your setup code here, to run once:
  keypad.addEventListener(key_event);
  keypad.setHoldTime(100);

  lcd.begin(cols, rows);
  lcd.cursor();
  lcd.blink();
  lcd_control_write("question number:");
}

void loop() {
  // put your main code here, to run repeatedly:
  keypad.getKey();
  
  if(state == 1)
  {
    state_1();
  }

  if (state == 4 && hold == "") {
    lcd_control_write("enter character");
    delay(100);
    return;
  }

  if(state == 4)
  {
    state_4();
  }

}

void state_1()
{
  int count = 0;
  while(true){
    lcd_control_write(NAME);
    colIndex++;
    if (colIndex == cols) {
      nextLine();
    }
    delay(1000);
    lcd.clear();
  }
}

void state_4()
{
  clear();
  while(true)
  {
    lcd_control_write(hold);
    colIndex = (colIndex + 1) % cols;
    rowIndex = (rowIndex + 1) % rows;
    delay(1000);
    lcd.clear();
  }
  
}

void key_event(KeypadEvent key)
{
  if(state == 0 && valid(key - '0'))
  {
    rowIndex = 1;
    lcd_control_write(String(key));
    delay(1000);
    clear();
    state = key - '0';
    return;
  }

  if (state == 4 && keypad.getState() == PRESSED) {
    hold = String(key);
    rowIndex = 1;
    lcd_control_write(hold);
    delay(1000);
    clear();
  }
  
  if(state == 2 && keypad.getState() == PRESSED)
  {
    if(key == '*')
    {
      nextLine();
      if(hold.equals(PASSWORD)) {
        hold = "";
        lcd_control_write("correct password");
      }
      else {
        hold = "";
        lcd_control_write("wrong password  ");
      }
      delay(500);
      clear();
      return;
    }
    hold += String(key);
    clear();
    lcd_control_write(hold);
    return;
  }

   if (state == 3 && keypad.getState() == PRESSED) {
    hold += String(key);
    clear();
    lcd_control_write(hold);
    switch(key)
    {
      case 'C':
        clear_calculate();
        break;
      case '=':
        printResult();
        break;
      case '+':
      case '-':
      case '*':
      case '/':
        operand = key;
        break;
      default:
        if (operand != '!') {
          first = first * 10 + key - '0';
        } else {
          second = second * 10 + key - '0';
        }
    }
   }
}

void clear_calculate() {
  first = second = 0;
  operand = '!';
  hold="";
  clear();
}

void printResult() {
  float result = 0;
  if (operand == '+')
    result = float(first) + float(second);
  if (operand == '/')
    result = float(second) / float(first);
  if (operand == '*')
    result = float(first) * float(second);
  if (operand == '-')
    result = float(second) - float(first);
  rowIndex = 1;
  colIndex = 0;
  lcd_control_write(String(result));
}

bool valid(byte state)
{
  return 0 < state && state < 5;
}

void lcd_control_write(String text)
{
  int tempColIndex = colIndex;
  int tempRowIndex = rowIndex;
  int cnt = cols - colIndex;
  if (rowIndex) {
    if (cnt < text.length()){
      int cc = text.length() - cnt;
      clear();
      lcd.setCursor(colIndex, rowIndex);
      for (int index = text.length() - cc; index < text.length(); index++) {
        lcd.print(text[index]);
        colIndex++;
        if (colIndex == cols) {
          nextLine();
        }
      }
      int numSpace = rows * cols - text.length();
      for (int i = 0; i < numSpace; i++) {
        lcd.print(" ");
        colIndex++;
        if (colIndex == cols) {
          nextLine();
        }
      }
      for (int index = 0; index < text.length() - cc; index++) {
        lcd.print(text[index]);
        colIndex++;
      }
    } else {
      lcd.setCursor(colIndex, rowIndex);
      for (int index = 0; index < text.length(); index++) {
        lcd.print(text[index]);
        colIndex++;
        if (colIndex == cols) {
          nextLine();
        }
      }
    }
  } else {
    if (cnt < text.length()){
      lcd.setCursor(colIndex, rowIndex);
      for (int index = 0; index < text.length(); index++) {
        lcd.print(text[index]);
        colIndex++;
        if (colIndex == cols) {
          nextLine();
        }
      }
    } else {
      lcd.setCursor(colIndex, rowIndex);
      for (int index = 0; index < text.length(); index++) {
        lcd.print(text[index]);
        colIndex++;
        if (colIndex == cols) {
          nextLine();
        }
      }
    }
  }
  colIndex = tempColIndex;
  rowIndex = tempRowIndex;
}

void clear()
{
  colIndex = rowIndex = 0;
  lcd.clear();
}

void nextLine()
{
  colIndex = 0;
  rowIndex = (rowIndex + 1) % rows;
  lcd.setCursor(colIndex, rowIndex);
}

