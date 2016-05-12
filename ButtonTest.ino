#include <Bounce2.h>

#define NUM_LEDS 6 
#define LOCKOUT_TIME 300 // amount of time that inputs from the other pins are locked out
#define DEBOUNCE_TIME 100 // amount of time that inputs from that pin are locked out

const int ledPins[NUM_LEDS] = {2,3,4,5,6,7};
const int buttonPins[NUM_LEDS] = {8,9,10,11,12,13};

#define SEQUENCE_COUNT 16
const int correctSequence[SEQUENCE_COUNT] = {0,1,2,3,4,5,0,1,2,3,4,5,0,1,2,3};
int currentSequence[SEQUENCE_COUNT] = {0,0,0,0,0,0,0,0,0,0,0,0};
int sequenceCounter = 0;


int previousState = HIGH;
int buttonIndex = 0;

  
Bounce debounce[NUM_LEDS];

int long previousTime = 0;

void setup(){
  
  for (int i = 0; i < NUM_LEDS; i++){
    
    debounce[i] = Bounce();
    debounce[i].attach(buttonPins[i]);
    debounce[i].interval(DEBOUNCE_TIME);
    
    pinMode(ledPins[i], OUTPUT);
    digitalWrite(ledPins[i], LOW);
    pinMode(buttonPins[i], INPUT_PULLUP);
  
  }
  
  Serial.begin(9600);
}

void loop(){

  
  // poll the state of the buttons, 
  for (int i = 0; i < NUM_LEDS; i++){
    debounce[i].update(); // if button i's state has changed
    
  }

  if (millis() - previousTime < LOCKOUT_TIME){
      return; 
  }

  for (int i = 0; i < NUM_LEDS; i++){
    if (debounce[i].read() == LOW){
      digitalWrite(ledPins[i], HIGH);
      pushButton(i);
    }else{
      digitalWrite(ledPins[i], LOW);
    }
  }
}


boolean checkSequence(){
  for (int i = 0; i < NUM_LEDS; i++){
    if (currentSequence[i] != correctSequence[i]){
      return false;
    }
  }
  return true;
}


void pushButton(int i){
  currentSequence[sequenceCounter] = i;
  if (sequenceCounter == SEQUENCE_COUNT - 1){
    if (checkSequence()){
      Serial.println("Correct Sequence");
    }else{
      Serial.println("Incorrect Sequence");
    }
  }
  sequenceCounter++;
}


