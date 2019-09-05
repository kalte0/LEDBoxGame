#include <Adafruit_NeoPixel.h>

//---------------Bit Stuff - Macros :) ----------------
#define set_bit(var, pin)   var |= 1<<(unsigned char) pin //set the pin in that register to 1. 
#define clr_bit(var, pin)   var &= ~(1<<(unsigned char)pin) // set the desired pin to 0
#define test_bit(var, pin)    ((var & (1<<(unsigned char) pin)) >> pin) // 
#define toggle_bit(var, pin)  var ^= 1<<(unsigned char) pin // name implies it pretty clearly 

#define NUMPIXELS 8 // Change when we get a longer strip. 

// --------------STATE NAMES-------------
#define JUMP 1
#define RUN 2
#define OFF 3

// ----------------------PINS-----------------
#define ANIM1 2 // ANIM1 should be the run animation in the front. Will be the first to activate during run animation. 
#define ANIM2 3
#define ANIMJUMP 4
#define PIN 5 // For Neopixel strip. 
#define BUTTON 6


long timeThis, timeLast; // Millis

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

int state; // variable for changing states
int binary; // store and read for the runObst function.
int hurdle; // used to store if the last obstacle is lit up. This wll be used to test collision w/ Froggy.
int buff; // this will be used to store if the button has been pushed and then released before allowing another jump.
int shift; // this will store which stage in the array to use next, incremented each time runObst is activated.

int Obst[11] = {0x80, 0x40, 0xA0, 0x50, 0x28, 0x14, 0x0A, 0x05, 0x02, 0x01, 0x00}; // First track of obstacles for the game.
//              128    64    160   80     40    20    10     5     2     1     0  // Decimal for the above Hex numbers.

void setup() {
  pinMode(ANIM1, OUTPUT);
  pinMode(ANIM2, OUTPUT);
  pinMode(ANIMJUMP, OUTPUT);
  pinMode(PIN, OUTPUT);
  pinMode(BUTTON, INPUT);
  Serial.begin(9600);
  pixels.begin();
  
for (int i = 0; i < 8; i++) {
      pixels.setPixelColor(i, pixels.Color(0, 0, 0)); // LED strip off
    }
  state = OFF;
}

void loop() {
  timeThis = millis();
  if (state != JUMP && hurdle == 1) {
    Serial.println("Wowoowoowowoowowwowowoowowowowowoowowwowowowowowowowowowowowowowwowowowowqowowoyousuck");
    Serial.println("Please keep playing our stupid game");
    for (int i = 0; i < 8; i++) {
      pixels.setPixelColor(i, pixels.Color(0, 0, 0));
    }
    pixels.show();
    state = OFF;
  }
  switch (state) {
    case OFF:
      shift = 0;
      if (digitalRead(BUTTON) == HIGH) {
        delay(50);
        digitalWrite(ANIM1, HIGH); // since the LEDS are toggling, need to set one to high before it starts.
        state = RUN;
      }
      break;

    case RUN:
      if (buff == 1) { // buff waits for button to be let go before allowing bcak to JUMP state.
        if ( digitalRead(BUTTON) == LOW) {
          buff = 0;
        }
      }
      if (buff == 0) {
        if (digitalRead(BUTTON) == HIGH ) {
          Serial.println("JUMP");
          timeLast = timeThis;
          state = JUMP ;
        }
      }
      if (timeThis - timeLast > 500) {
        timeLast = timeThis;
        digitalWrite(ANIM1, !(digitalRead(ANIM1))); // toggle the two frames of animation.
        digitalWrite(ANIM2, !(digitalRead(ANIM2)));
        runObst();
      }
      break;

    case JUMP:
      digitalWrite(ANIM1, LOW);
      digitalWrite(ANIM2, LOW);
      digitalWrite(ANIMJUMP, HIGH);
      if (timeThis - timeLast > 500) {
        Serial.println("To RUN");
        digitalWrite(ANIMJUMP, LOW);
        digitalWrite(ANIM1, HIGH);
        buff = 1;
        runObst();
        timeLast = timeThis;
        state = RUN;
      }
      break;
  }
  timeThis = timeLast;
}



int runObst() { // runs one stage of the obstacles based on the Obst[] array.
  binary = Obst[shift];
  shift++;
  if (shift == 11) { // since the array is only 11 values long, (#0-10), once shift equals 11, set it back to 0.
    shift = 0;
  }
 // Serial.print(binary, HEX); Serial.print('\t'); Serial.println(shift); print info
  for ( int x = 0; x < 8; x++ ) { // this for loops writes each LED in the strip based on var binary.
    
    if (test_bit(binary, x) == 1) {
      pixels.setPixelColor(x, pixels.Color(0, 150, 0));
      if (x == 8) {
        hurdle = 1; 
      }
    }
    else {
      pixels.setPixelColor(x, pixels.Color(0, 0, 0 ));
      if (x == 8) {
        hurdle = 0;
      }
    }
    pixels.show();
    delay(10);
  }
}







/*
  timeThis = millis();

  switch (state) {
    case OFF:
    if (timeThis - timeLast > 50) {
      if (digitalRead(BUTTON) == HIGH) {
       Serial.println("Yupperoonies");
       state = RUN;
      }
       timeLast = timeThis;
    }

      break;

    case JUMP:

      break;

    case RUN:

      break;
  }
  }


  /*  for(int i=0;i<NUMPIXELS;i++){
    pixels.setPixelColor(i, pixels.Color(0,150,0));
    pixels.show();
  } */ // Copy from later, Neopixel code .
