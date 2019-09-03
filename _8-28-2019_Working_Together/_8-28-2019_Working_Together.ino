#include <Adafruit_NeoPixel.h>

//---------------Bit Stuff - Macros :) ----------------
#define set_bit(var, pin)   var |= 1<<(unsigned char) pin //set the pin in that register to 1. 
#define clr_bit(var, pin)   var &= ~(1<<(unsigned char)pin) // set the desired pin to 0
#define test_bit(var, pin)    ((var & (1<<(unsigned char) pin)) >> pi n) // 
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
int delayval = 500; // for Neo pixel

int state; // variable for changing states
int binary;

int Obst[11] = {0x80, 0x40, 0xA0, 0x50, 0x28, 0x14, 0x0A, 0x05, 0x02, 0x01, 0x00};

void setup() {
  pinMode(ANIM1, OUTPUT);
  pinMode(ANIM2, OUTPUT);
  pinMode(ANIMJUMP, OUTPUT);
  pinMode(PIN, OUTPUT);
  pinMode(BUTTON, INPUT);
  Serial.begin(9600);
  pixels.begin();

}

void loop() {
  binary = Obst[3];
  Serial.println(binary, HEX);
  for ( int x = 0; x < 8; x++ ) {
if (test_bit(binary, x) == 1) {
  Serial.print(x); Serial.print('\t'); Serial.println("Yup"); 
}
  }
}

/*int RunObst(int steps) {
  binary = Obst[steps];
  Serial.println(binary, HEX);
  for ( int x = 0; x < 8; x++ ) { // look later to see if the 8 is what we want for the end bound

  }
  delay(50);*/





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
