#include <Bounce2.h>

#define NUM_TIMER_LEDS 3
#define NUM_LEDS 6 
#define LOCKOUT_TIME 300 // amount of time that inputs from the other pins are locked out
#define DEBOUNCE_TIME 200 // amount of time that inputs from that pin are locked out
#define RESET_TIME 5 // amount of time between button presses, if exceeded -> fail sequence

const int ledPins[NUM_LEDS] = {2,3,4,5,6,7};
const int buttonPins[NUM_LEDS] = {8,9,10,11,12,13};
const int timerLeds[NUM_TIMER_LEDS] = {14,15,16}; 

#define SEQUENCE_COUNT 16
const int correctSequence[SEQUENCE_COUNT] = {0,1,2,3,4,5,0,1,2,3,4,5,0,1,2,3};
int currentSequence[SEQUENCE_COUNT] = {0,0,0,0,0,0,0,0,0,0,0,0};
int sequenceCounter = 0;
int resetTimer = RESET_TIME;
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

  for (int i = 0; i < NUM_TIMER_LEDS; i++){
    pinMode(timerLeds[i], OUTPUT);
    digitalWrite(timerLeds[i], LOW);
  }

    // INITIALIZE TIMER INTERRUPTS
  cli(); // disable global interrupt
  TCCR1A = 0; // set entire TCCR1A register to 0
  TCCR1B = 0; // same for TCCR1B
  OCR1A = 15624; // set compare match register to desired timer count. 16 MHz with 1024 prescaler = 15624 counts/s
  // Bit: 7,6,5,4,3,2,1,0
  // WGM12: 3, CS10: 1, CS12: 2
  // the value on the left by the number of bits on the right
  TCCR1B |= (1 << WGM12); // turn on CTC mode. clear timer on compare match
  TCCR1B |= (1 << CS10); // Set CS10 and CS12 bits for 1024 prescaler
  TCCR1B |= (1 << CS12);
  // basically TCCR1B is 0000 1110
  TIMSK1 |= (1 << OCIE1A); // enable timer compare interrupt

  sei(); // enable global interrupts
  
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
      previousTime = millis();
      break;
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

  // display the index of the button that was just pressed
  Serial.print(i);  
    
  if (sequenceCounter == SEQUENCE_COUNT - 1){
    
    if (checkSequence()){
      Serial.println("Correct Sequence");
    }else{
      Serial.println("Incorrect Sequence");
    }
    
    reset();
  }
  
  sequenceCounter++;
}

void reset(){
  sequenceCounter = 0;
  for (int i = 0; i < NUM_LEDS; i++){
    currentSequence[i] = 0;
  }
}


ISR(TIMER1_COMPA_vect){ 
   resetTimer--;
   
   if (resetTimer == 0){
      Serial.println("Reset, waited too long in between "); // time between button presses too long
      resetGame();
   }else if (resetTimer == RESET_TIME){
    
   }else if (resetTimer == RESET_TIME - 1){
    
   }else if (resetTimer == RESET_TIMER -2){
    
   }
}



