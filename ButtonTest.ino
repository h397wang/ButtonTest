#include <Bounce2.h>

#define NUM_LEDS 6
#define LOCKOUT_TIME 300 // ignore inputs from the other pins
#define DEBOUNCE_TIME 100 // time between button presses

const int ledPins[NUM_LEDS] = {2,3,4,5,6,7};
const int buttonPins[NUM_LEDS] = {8,9,10,11,12,13};

#define SEQUENCE_COUNT 12
const int correctSequence[SEQUENCE_COUNT] = {0,1,2,3,4,5,0,1,2,3,4,5};
int currentSequence[SEQUENCE_COUNT] = {0,0,0,0,0,0,0,0,0,0,0,0};
int sequenceCounter = 0;

int buttonState = LOW;
int previousButtonState = LOW;

Bounce debounce[NUM_LEDS];

int long previousTime = 0;

void setup(){
  
  for (int i = 0; i < NUM_LEDS; i++){
    
    debounce[i] = Bounce();
    debounce[i].attach(buttonPins[i]);
    debounce[i].interval(DEBOUNCE_TIME);
    
    pinMode(ledPin[i], OUTPUT);
    digitalWrite(ledPins[i], LOW);
    pinMode(buttonPins[i], INPUT_PULLUP);
  
  }
  
  Serial.begin(9600);
}

void loop(){

  int buttonIndex = 0; // that was pressed
  
  for (int i = 0; i < NUM_LEDS; i++){
    if (debounce[i].update() == true){ // button i state has changed
      break;
    }
  }

  if (millis() - previousTime < LOCKOUT_TIME){
      return; 
  }
  
  for (int i = 0; i < NUM_LEDS; i++){
    if (debounce[i].read() == false){ // then the button was truly pressed
      pushButton(i);
      digitalWrite(i, HIGH);
      previousTime = millis();
      break;
    }
  }
  
}

void buttonPress(int i){
  currentSequence[sequenceCounter] = i;
  if (sequenceCounter == SEQUENCE_LENGTH - 1){
    if (checkSequence()){
      Serial.println("Correct Sequence");
    }else{
      Serial.println("Incorrect Sequence");
    }
}


void boolean checkSequence(){
  for (int i = 0; i < NUM_LEDS; i++){
    if (currentSequence[i] != correctSequence[i]){
      return false;
    }
  }
  return true;
}
