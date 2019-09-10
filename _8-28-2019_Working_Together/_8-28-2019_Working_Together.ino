#include <Adafruit_NeoPixel.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

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

// -------------------OLED DISPLAY---------------
#define OLED_MOSI   9
#define OLED_CLK   10
#define OLED_DC    11
#define OLED_CS    12
#define OLED_RESET 13
Adafruit_SSD1306 display(OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);

long timeThis, timeLast, timeLast2; // Millis

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

int state; // variable for changing states
int binary; // store and read for the runObst function.
int hurdle = 0; // used to store if the last obstacle is lit up. This will be used to test collision w/ Froggy.
int buff; // this will be used to store if the button has been pushed and then released before allowing another jump.
int shift; // this will store which stage in the array to use next, incremented each time runObst is activated.
int score = 0;
int highScore = 0;
int scoreWait; // this helps the program only count a point once. Once a point is won, this becomes 1. Then, once you're no longer jumping it returns to 0. Points are only counted when it is at 0, which is only once.
int idle; // helps program w/ timing millis for idle Animations
int maxShift; // sets the highest shift value possible, for use in runObst function
int y;

// guide to making obstacles: the beginning of the array will be activated first,
// in binary: 00001, or 1, will display as closest to the base of the strip, or to the player.
// basically, make the ones move from left to right here, and it will move correctly in the game.
int obstIdle[8] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};
int obst1[17] = {0x80, 0x40, 0x20, 0x10, 0x88, 0x44, 0x22, 0x91, 0x48, 0x24, 0x12, 0x09, 0x04, 0x02, 0x01};

void setup() {
  pinMode(ANIM1, OUTPUT);
  pinMode(ANIM2, OUTPUT);
  pinMode(ANIMJUMP, OUTPUT);
  pinMode(PIN, OUTPUT);
  pinMode(BUTTON, INPUT);
  Serial.begin(9600);
  display.begin(SSD1306_SWITCHCAPVCC);
  pixels.begin();

  for (int i = 0; i < 8; i++) {
    pixels.setPixelColor(i, pixels.Color(0, 0, 0)); // LED strip off
  }
  pixels.show();

  display.clearDisplay();

  state = OFF;
  idleAnimCircle();
  digitalWrite(ANIM1, HIGH);
  shift = 0;
}

void loop() {
  timeThis = millis();

  //-----------------------------------------Failure Detection----------
  if (state != JUMP && state != OFF && hurdle == 1) {
    Serial.println("Wowoowoowowoowowwowowoowowowowowoowowwowowowowowowowowowowowowowwowowowowqowowoyousuck");
    Serial.println("Please keep playing our stupid game");
    digitalWrite(ANIM1, LOW);
    digitalWrite(ANIM2, LOW);
    digitalWrite(ANIMJUMP, LOW);
    for (int i = 0; i < 8; i++) {
      pixels.setPixelColor(i, pixels.Color(0, 0, 0));
    }
    pixels.show();
    hurdle = 0;
    if (score > highScore) {
      highScore = score;
    }
    score = 0;
    idleAnimCircle();
    digitalWrite(ANIM1, HIGH);
    shift = 0;
    Serial.println("Fail");
    state = OFF;
  }

  //------------------------------------------ScoreKeeping----------------
  if (state == JUMP && hurdle == 1 && scoreWait == 0) {
    score++;
    Serial.println(score);

    scoreWait = 1;
  }
  if (state != JUMP && scoreWait == 1) {
    scoreWait = 0;
  }

  switch (state) {
    case OFF: //-------------------------------------------------------------------------- OFF

      if (digitalRead(BUTTON) == HIGH) {
        delay(50);
        digitalWrite(ANIM1, HIGH); // since the LEDS are toggling, need to set one to high before it starts.
        timeLast = timeThis;
        timeLast2 = timeThis;
        Serial.println("To RUN");
        shift = 0;
        startAnim(); 
        for (int i = 0; i < 8; i++) {
          pixels.setPixelColor(i, pixels.Color(0, 0, 0)); // LED strip off
        }
        pixels.show();
        
        state = RUN;
      }
      idleAnim();
      break;


      

    case RUN://--------------------------------------------------------------------------  RUN
      if (buff == 1) { // buff waits for button to be let go before allowing back to JUMP state.
        if ( digitalRead(BUTTON) == LOW) {
          buff = 0;
        }
      }
      if (buff == 0) {
        if (digitalRead(BUTTON) == HIGH ) {
          timeLast = timeThis;
          timeLast2 = timeThis;
          Serial.println("to JUMP");
          state = JUMP ;
        }
      }
      if (timeThis - timeLast > 500) {
        timeLast = timeThis;
        digitalWrite(ANIM1, !(digitalRead(ANIM1))); // toggle the two frames of animation.
        digitalWrite(ANIM2, !(digitalRead(ANIM2)));
      }
      if (timeThis - timeLast2 > 400) {
        runObst();
        timeLast2 = timeThis;
      }
      break;


    case JUMP://--------------------------------------------------------------------------  JUMP
      digitalWrite(ANIM1, LOW);
      digitalWrite(ANIM2, LOW);
      digitalWrite(ANIMJUMP, HIGH);

      if (timeThis - timeLast2 > 400) {
        runObst();
        timeLast2 = timeThis;
      }
      if (timeThis - timeLast > 500) {
        digitalWrite(ANIMJUMP, LOW);
        digitalWrite(ANIM1, HIGH);
        buff = 1;
        runObst();
        timeLast = timeThis;
        Serial.println("To RUN");
        state = RUN;
      }
      break;
  }
  timeThis = timeLast;
}


int runObst() {// runs one stage of the obstacles based on the obst[] array.
  binary = obst1[shift];
  shift++;
  if (shift == 17) { // since the array is only 17 values long, (#0-10), once shift equals 11, set it back to 0.
    shift = 0;
  }

  // Serial.print(binary, HEX); Serial.print('\t'); Serial.println(shift); // print info
  for ( int x = 0; x < 8; x++ ) { // this for loops writes each LED in the strip based on var binary.

    if (test_bit(binary, x) == 1) {
      pixels.setPixelColor(x, pixels.Color(0, 150, 0));
      if (x == 0) {
        hurdle = 1;
      }
    }
    else {
      pixels.setPixelColor(x, pixels.Color(0, 0, 0 ));
      if (x == 0) {
        hurdle = 0;
      }
    }
    pixels.show();
    delay(10);
  }
}

int idleAnim() {
  if (timeThis - timeLast > 8000) { // the millis here will be how long the TEXT will last
    idleAnimCircle();
    idle = 0;
    timeLast = timeThis;
  }
  if (timeThis - timeLast > 1000 && idle == 0) { // the millis time here is how long the CIRCLE will last
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(BLACK, WHITE); //                  Text Animation
    display.setCursor(0, 0);
    display.print("Froggy Run");
    display.setTextColor(WHITE, BLACK);
    display.setCursor(10, 18);
    display.print("High:"); display.println(highScore);
    display.display();
    display.startscrollleft(0x00, 0x0F);
    idle = 1;
    timeLast = timeThis;
  }

  if (timeThis - timeLast2 > 500) {
    runObst();
    if (hurdle == 1) {
      digitalWrite(ANIM1, LOW);
      digitalWrite(ANIM2, LOW);
      digitalWrite(ANIMJUMP, HIGH);
      y = 1;
    }
    else if (y == 1) {
      digitalWrite(ANIM1, HIGH);
      digitalWrite(ANIM2, LOW);
      digitalWrite(ANIMJUMP, LOW);
      y = 0;
    }
    if (hurdle == 0 && y == 0) {
      digitalWrite(ANIM1, !(digitalRead(ANIM1))); // toggle the two frames of animation.
      digitalWrite(ANIM2, !(digitalRead(ANIM2)));
    }
    timeLast2 = timeThis;
  }

}

int idleAnimCircle() {
  display.clearDisplay();
  for (int16_t i = 0; i < max(display.width(), display.height()) / 2; i += 2) {
    display.drawCircle(display.width() / 2, display.height() / 2, i, WHITE);
    display.display();
    delay(2);
  }
  display.drawPixel(display.width() / 2, display.height() / 2, BLACK); //           Circles Animation
  for (int16_t i = 0; i < max(display.width(), display.height()) / 2; i += 2) {
    display.drawCircle(display.width() / 2, display.height() / 2, i, BLACK);
    display.display();
    delay(2);
  }
}

int startAnim() {
   display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(BLACK, WHITE); //                  Text Animation
    display.setCursor(0, 0);
    display.print("START GAME");
    display.display(); 
}

/*  idleAnimCircle();
    digitalWrite(ANIM1, HIGH);
    idle = 0;
    shift = 0; */ // wherever the code moves to off, include these three so the animation will work correctly.



