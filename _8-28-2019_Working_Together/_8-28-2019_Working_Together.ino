#include <Adafruit_NeoPixel.h>

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

int state; // variable for changing states.\

int Obst[8] 

void setup() {
  pinMode(ANIM1, OUTPUT);
  pinMode(ANIM2, OUTPUT);
  pinMode(ANIMJUMP, OUTPUT);
  pinMode(PIN, OUTPUT);
  pinMode(BUTTON, INPUT);
  Serial.begin(9600);
  pixels.begin();

  state = OFF;
}

void loop() {
  timeThis = millis();
  if (timeThis - timeLast > 1000) {
    for (int i = 0; i < NUMPIXELS; i++) {
      pixels.setPixelColor(i, pixels.Color(0, 0, 150)); //make some major changes to this, can't have just one for loop that does it all for the obstacles. 
      pixels.show();
    }
    timeLast = timeThis;
  }

  switch (state) {
    case OFF:

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
