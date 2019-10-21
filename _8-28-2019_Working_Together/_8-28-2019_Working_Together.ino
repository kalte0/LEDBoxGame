// IDEAS TO ADD IN FUTURE:
/* - nickname save with high score
    - change obst color with level change
*/


#include <Adafruit_NeoPixel.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <math.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

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
#define NICKNAME 4

// ----------------------PINS-----------------
#define ANIM1 2 // ANIM1 should be the run animation in the front. Will be the first to activate during run animation. 
#define ANIM2 3
#define ANIMJUMP 4
#define PIN 6 // For Neopixel strip. 
#define BUTTON 5
#define PIEZO 7

// -------------------OLED DISPLAY---------------
#define OLED_MOSI  9
#define OLED_CLK   10
#define OLED_DC    11
#define OLED_CS    12
#define OLED_RESET 13
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT,
                         OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);

 //------------- key_read defines -----
 
#define KEY_PRESSED HIGH //state of key being pressed
#define KEY_NO_PRESS 0 //key not pressed
#define KEY_SHORT_PRESS 1 //key pressed short
#define KEY_LONG_PRESS 2 //key pressed long
#define KEY_DURATION 500 //cycle count, minimum threshold to test for long presses

unsigned long key_read(unsigned char pin)  {
  unsigned long count = 0;
  if (digitalRead(pin) != KEY_PRESSED) return KEY_NO_PRESS; //key not pressed
  //key is pressed
  while (digitalRead(pin) == KEY_PRESSED) {
    count += 1; //increment count if key is continuously pressed
    delay(1);
  }
  if (count > KEY_DURATION) return KEY_LONG_PRESS;
  else return KEY_SHORT_PRESS;
}

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
int z;
int track = 0; // chooses which track to run.
int delayVal = 500; // will control how fast the game runs.
int level = 1; // level is a series of 3 tracks, and have different speeds.
int downWait; // turns to one when there is an obtacle under the jump, tells the player to immediatley go back down when they have gotten over that obstacle.

// guide to making obstacles: the beginning of the array will be activated first,
// in binary: 00001, or 1, will display as closest to the base of the strip, or to the player.
// basically, make the ones move from left to right here, and it will move correctly in the game.
int obstIdle[8] = {0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01};                                         //far---->player. full obst track.
int obst1[15] = {0x80, 0x40, 0x20, 0x10, 0x88, 0x44, 0x22, 0x91, 0x48, 0x24, 0x12, 0x09, 0x04, 0x02, 0x01}; //10001001
int obst2[15] = {0x80, 0x40, 0x20, 0x90, 0x48, 0xA4, 0x52, 0xA9, 0x54, 0x2A, 0x15, 0x0A, 0x05, 0x02, 0x01}; //10101001
int obst3[16] = {0x80, 0x40, 0xA0, 0x50, 0x28, 0x14, 0x8A, 0x45, 0xA2, 0x51, 0x28, 0x14, 0x0A, 0x05, 0x02, 0x01};//101000101
int levelStages[9] = {1, 2, 3, 3, 2, 1, 2, 1, 3};
//                    1        2        3


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
  display.display();

  tone(PIEZO, 0, 1000);
  state = OFF;
  digitalWrite(ANIM1, HIGH);
  shift = 0;
}

void loop() {
  timeThis = millis();


  //-----------------------------------------Failure Detection----------
  if (state != JUMP && state != OFF && hurdle == 1) {
    digitalWrite(ANIM1, LOW);
    digitalWrite(ANIM2, LOW);
    digitalWrite(ANIMJUMP, LOW);
    for (int i = 0; i < 8; i++) {
      pixels.setPixelColor(i, pixels.Color(0, 0, 0));
    }
    pixels.show();
    failAnim();
    hurdle = 0;

    score = 0;
    idleAnimText();
    digitalWrite(ANIM1, HIGH);
    shift = 0;
    Serial.println("Fail");
    state = OFF;
  }

  if (timeThis - timeLast2 > delayVal) { // how long the obstacles will run
    runObst();
    timeLast2 = timeThis;
  }

  //------------------------------------------ScoreKeeping----------------
  if (state == JUMP && hurdle == 1 && scoreWait == 0) {
    score++;
    pointAnim();
    tone(PIEZO, 261.63, 150);
    //Serial.println(score)
    scoreWait = 1;
  }
  if (state != JUMP && scoreWait == 1) {
    scoreWait = 0;
    tone(PIEZO, 392.00, 200);
  }

  switch (state) {
    case OFF: //-------------------------------------------------------------------------- OFF

      if (digitalRead(BUTTON) == HIGH) {
        delay(50);
        for (int i = 0; i < 8; i++) {
          pixels.setPixelColor(i, pixels.Color(0, 0, 0)); // LED strip off
        }
        pixels.show();
        // Serial.println("To RUN");
        shift = 0;
        buff = 0;
        startAnim();
        timeLast = timeThis;
        timeLast2 = timeThis;
        track = 1;
        state = RUN;
      }
      else {
        idleAnim();
      }

      break;




    case RUN://--------------------------------------------------------------------------  RUN
      if (buff == 1 && digitalRead(BUTTON) == LOW) { // buff waits for button to be let go before allowing back to JUMP state.
        buff = 0;
      }
      if (buff == 0 && digitalRead(BUTTON) == HIGH) {
        timeLast = timeThis;
        // Serial.println("to JUMP");
        state = JUMP ;
      }
      if (timeThis - timeLast > delayVal) {//how long each frame of animation will run.
        timeLast = timeThis;
        digitalWrite(ANIM1, !(digitalRead(ANIM1))); // toggle the two frames of animation.
        digitalWrite(ANIM2, !(digitalRead(ANIM2)));
      }
      /*   if (timeThis - timeLast2 > delayVal - (delayVal * 0.2)) {
           runObst();
           timeLast2 = timeThis;
         }*/
      break;


    case JUMP://--------------------------------------------------------------------------  JUMP
      digitalWrite(ANIM1, LOW);
      digitalWrite(ANIM2, LOW);
      digitalWrite(ANIMJUMP, HIGH);
      if (hurdle == 1) {
        downWait = 1;
      }
      if (timeThis - timeLast > delayVal * 3 or downWait == 1 && hurdle == 0) { //how long the jump will last.
        digitalWrite(ANIMJUMP, LOW);
        digitalWrite(ANIM1, HIGH);
        buff = 1;
        timeLast = timeThis;
        // Serial.println("To RUN");
        state = RUN;
        downWait = 0;
      }
      break;

    case NICKNAME:
      display.clearDisplay();
      
      break;
  }
  timeThis = timeLast;
}


int runObst() {

  if (state == OFF) {
    track = 0;
    binary = obstIdle[shift];
    maxShift = 8;
    level = 1;
  }

  else if (state != OFF) {
    if (track == 1) {
      binary = obst1[shift];
      maxShift = 15;
    }
    if (track == 2) {
      if (level == 1) {
        level = 2;
      }
      binary = obst2[shift];
      maxShift = 15;
    }
    if (track == 3) {
      binary = obst3[shift];
      maxShift = 16;
    }
  }

  double a = pow(0.80, level);
  delayVal = 850 * a;

  Serial.print("Level:"); Serial.print(level); Serial.print('\t'); Serial.println(delayVal);

  shift++;
  if (shift == maxShift) { // since the array is only 17 values long, (#0-10), once shift equals 11, set it back to 0.
    shift = 0;
    track++;
    if (track == 4) {
      track = 1;
      level++;
      //Later, add an animation for increased level.
    }
    // Serial.println(track);
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
  }
}


int idleAnim() {
  if (timeThis - timeLast > 8000) { // the millis here will be how long the TEXT will last
    idleAnimCircle();
    idle = 0;
    timeLast = timeThis;
  }
  if (timeThis - timeLast > 1000 && idle == 0) { // the millis time here is how long the CIRCLE will last
    idleAnimText();
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

int idleAnimText() {
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
}

int startAnim() {
  for (int x = 0; x < 5; x++) {
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(BLACK, WHITE); //                  Text Animation for startup
    display.setCursor(0, 0);
    display.print("START GAME");
    display.display();
    delay(100);
    display.clearDisplay();
    display.setTextColor(WHITE, BLACK);
    display.setCursor(0, 0);
    display.print("START GAME");
    display.display();
    delay(100);
  }
  tone(PIEZO, 261.63);
  delay(125);
  tone(PIEZO, 329.63);
  delay(125);
  tone(PIEZO, 392.00);
  delay(200);
  noTone(PIEZO);
  display.clearDisplay();
  display.setTextColor(WHITE, BLACK);
  display.setCursor(0, 10);
  display.print("GOOD LUCK!");
  display.display();
  digitalWrite(ANIM1, HIGH);
  digitalWrite(ANIM2, LOW);
  digitalWrite(ANIMJUMP, LOW);
}

int failAnim() {

  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE); //                  Text Animation for failure
  display.setCursor(10, 8);
  display.print("Game Over");
  display.display();
  tone(PIEZO, 329.63); // E4
  delay(100);
  tone(PIEZO, 261.63); // C4
  delay(100);
  tone(PIEZO, 220.00); // A3
  delay(100);
  noTone(PIEZO);
  delay(1200);
  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(20, 8);
  display.print("Score:"); display.println(score);
  display.display();
  delay(2000);

  if (score > highScore) {
    highScore = score;

    for (int x = 0 ; x < 5; x++) {
      display.clearDisplay();
      display.setTextSize(2);
      display.setCursor(0, 8);
      display.setTextColor(WHITE);
      display.print("HIGHSCORE");
      display.display();
      tone(PIEZO, 261.63); // C4
      delay(100);
      display.clearDisplay();
      display.setTextSize(2);
      display.setCursor(0, 8);
      display.setTextColor(BLACK, WHITE);
      display.print("HIGHSCORE");
      display.display();
      tone(PIEZO, 392.00); // G4
      delay(100);
    }
    noTone(PIEZO);
    state = NICKNAME;
  }
}

int pointAnim() {
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.print("Level:"); display.print(level);
  display.setCursor(0, 18);
  display.print("Score:"); display.print(score);
  display.display();
}




/*  idleAnimText();
    digitalWrite(ANIM1, HIGH);
    idle = 0;
    shift = 0; */ // wherever the code moves to off, include these three so the animation will work correctly.



