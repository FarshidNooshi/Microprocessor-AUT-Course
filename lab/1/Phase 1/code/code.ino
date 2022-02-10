#include <assert.h>

#define REVERSE -1

const int polling_pin[] = {12, 11, 10, 9}, len_poll = sizeof(polling_pin)/sizeof(*polling_pin);
const int leds[] = {0, 1, 2, 3, 4, 5, 6, 7}, len_leds = sizeof(leds)/sizeof(*leds);
int switchs_state[] = {1, 1, 1, 1};//0 -> on, 1 -> off
int index_pin = 0, state = 0;

const char *name = "Farshid";

void setup() {
  // put your setup code here, to run once:
  for(int i = 0; i < len_leds; i++){
    pinMode(leds[i], OUTPUT);
  }

  for(int i = 0; i < len_poll; i++){
    pinMode(polling_pin[i], INPUT);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  
  for(int i = 0; i < len_poll; i++){
    switchs_state[i] = digitalRead(polling_pin[i]);
  }
  
  find_state_to_execute();
}

bool check_not_conflict(int index_select) {
   for(int i = 0; i < len_poll; i++){
      if(i != index_select ^ switchs_state[i] == HIGH){
        return false;
      }
   }
   return true;
}

void find_state_to_execute() {
  if(check_not_conflict(0) && state == 0){
    run_state_1();
  } else if(check_not_conflict(1) && state == 0){
    state = 1;
    run_state_2();
  } else if(check_not_conflict(2)){
    state = 0;
    run_state_3();
  } else if(check_not_conflict(3) && state == 0){
    run_state_4();
  }
  
}

void run_state_1() {
  digitalWrite(leds[index_pin], HIGH);
  delay(500);
  digitalWrite(leds[index_pin], LOW);
  index_pin = (index_pin + 1) % len_leds;
}

void run_state_2() {
  int size_name = strlen(name);
  while(size_name--)
  {
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
  while(!digitalRead(polling_pin[3])){
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
