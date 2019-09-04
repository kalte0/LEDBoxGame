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
#define BUFF 4

// ----------------------PINS-----------------
#define ANIM1 2 // ANIM1 should be the run animation in the front. Will be the first to activate during run animation. 
#define ANIM2 3
#define ANIMJUMP 4
#define PIN 5 // For Neopixel strip. 
#define BUTTON 6


long timeThis, timeLast; // Millis

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
int delayval = 500; // for Neo pixel

int state; // variable for changing states
int binary; // store and read for the runObst function. 
int 

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

  state = RUN;
}

void loop() {
  timeThis = millis();

  switch (state) {
    
    case RUN:
      if (digitalRead(BUTTON) == HIGH ) {
        Serial.println("JUMP");
        state = JUMP ;
      }
      if (timeThis - timeLast > 500) { // Every half second:
        timeLast = timeThis;
        digitalWrite(ANIM1, !(digitalRead(ANIM1))); // toggle the two frames of animation.
        digitalWrite(ANIM2, !(digitalRead(ANIM2)));
        state = RUN2;
      }
      break;

    case JUMP:
      digitalWrite(ANIM1, LOW);
      digitalWrite(ANIM2, LOW);
      digitalWrite(ANIMJUMP, HIGH);
      if (timeThis - timeLast > 500) {
        timeLast = timeThis;
        digitalWrite(ANIMJUMP, LOW);
     
      }

      break;



    case
      }
  timeThis = timeLast;
}



void int runObst() { // function for runObst
  for (int y = 0; y < 11; y++) {
    binary = Obst[y];
    Serial.println(binary, HEX);
    runObst();
    for ( int x = 0; x < 9; x++ ) {
      if (test_bit(binary, x) == 1) {
        if (x == 9) {
          digitalWrite(ANIM1, HIGH); 
        }
        pixels.setPixelColor(x, pixels.Color(0, 150, 0));
      }
      else {
        pixels.setPixelColor(x, pixels.Color(0, 0, 0 ));
      }
      pixels.show();
      delay(10);
    }
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
    delay(delayval);
  } */ // Copy from later, Neopixel code .
