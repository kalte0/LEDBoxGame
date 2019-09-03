#include <Adafruit_NeoPixel.h>

//----------------BIT STUFF----------------------
#define set_bit(var, pin)    var |= 1<<(unsigned char) pin)
#define clr_bit(var, pin)   var &= ~(1<<(unsigned char)pin)
#define test_bit(var, pin)    ((var & (1<<(unsigned char) pin)) >> pin)
#define toggle_bit(var, pin)  var ^= 1<<(unsigned char) pin)


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
int Binary;

int Obst[11] = { 0, 1, 2, 5, 10, 20 , 40 , 80, 160, 64, 128};

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
  runObst 

RunObst (int slice) { 

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
