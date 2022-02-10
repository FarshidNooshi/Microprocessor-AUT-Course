#include <assert.h>

#define REVERSE -1

const int leds[] = {0, 1, 2, 3, 4, 5, 6, 7}, len_leds = sizeof(leds)/sizeof(*leds);
const int interrupts_pin[] = {18, 19, 20, 21}, len_interrupts = sizeof(interrupts_pin)/sizeof(*interrupts_pin);
volatile int button_flag[] = {0, 0, 0, 0};
int index_pin = 0, state = 1;

const char *name = "Farshid";

void setup() {
  // put your setup code here, to run once:
  for(int i = 0; i < len_leds; i++)
  {
    pinMode(leds[i], OUTPUT);
  }

  for(int i = 0; i < len_interrupts; i++)
  {
    pinMode(interrupts_pin[i], INPUT_PULLUP);
  }

  attachInterrupt(digitalPinToInterrupt(interrupts_pin[0]), change_button_1, FALLING);
  attachInterrupt(digitalPinToInterrupt(interrupts_pin[1]), change_button_2, FALLING);
  attachInterrupt(digitalPinToInterrupt(interrupts_pin[2]), change_button_3, FALLING);
  attachInterrupt(digitalPinToInterrupt(interrupts_pin[3]), change_button_4, FALLING);
  
}

void loop() {
  // put your main code here, to run repeatedly:
  if(button_flag[0] && state){
    run_state_1();
    button_flag[0] = 0;
  } else if(button_flag[1] && state){
    run_state_2();
    state = 0;
    button_flag[1] = 0;
  } else if(button_flag[2]){
    run_state_3();
    state = 1;
    button_flag[2] = 0;
  } else if(button_flag[3] && state) {
    run_state_4();
    button_flag[3] = 0;
  }
}

void change_button_1() {
  button_flag[0] = 1;
}

void change_button_2() {
  button_flag[1] = 1;
}

void change_button_3() {
  button_flag[2] = 1;
}

void change_button_4() {
  button_flag[3] = 1;
}

void run_state_1() {
  digitalWrite(leds[index_pin], HIGH);
  delay(500);
  digitalWrite(leds[index_pin], LOW);
  index_pin = (index_pin + 1) % len_leds;
}

void run_state_2() {
  int size_name = strlen(name);
  while(size_name--){
    change_state(HIGH);
    delay(200);
    change_state(LOW);
    delay(200);
  }
  change_state(HIGH);
  delay(200);
}

void run_state_3() {
  change_state(LOW);
}

void run_state_4() {
  while(!digitalRead(interrupts_pin[3])){
    change_state(REVERSE);
    delay(5000);
  }
  change_state(LOW);
}

void change_state(int new_state) {
  for(int j = 0; j < len_leds; j++){
      if(new_state == REVERSE){
        int status = digitalRead(leds[j]);
        digitalWrite(leds[j], !status);
      }
      else {
        digitalWrite(leds[j], new_state);
      }
    }
}
