#include <Bounce2.h>

#define NUM_PINS 6
#define LOCKOUT_TIME 300
#define DEBOUNCE_TIME 100

const int ledPins = 5;
const int buttonPins = 11;
boolean buttonState = LOW;
boolean previousButtonState = LOW;

int long previousTime = 0;

void setup(){
  
 
  pinMode(ledPins, OUTPUT);
  digitalWrite(ledPins, LOW);
  pinMode(buttonPins, INPUT);

  /*
  debounce = Bounce();
  debounce.attach(buttonPins);
  debounce.interval(DEBOUNCE_TIME);
  */
  
  Serial.begin(9600);
}

void loop(){
  
  buttonState = digitalRead(buttonPins);
  Serial.println("Button State: " + (int)buttonState);

  if (buttonState != previousButtonState){
    if (millis() - previousTime < DEBOUNCE_TIME){
      return; 
    }
  }
  
  if (buttonState == HIGH) {
    // turn LED on:
    digitalWrite(ledPins, HIGH);
  } else {
    // turn LED off:
    digitalWrite(ledPins, LOW);
  }
  
}

void buttonPress(int i){
  
}

