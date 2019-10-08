
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_NeoPixel.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

#define ANIM1 2
#define ANIM2 3
#define ANIMJUMP 4

#define PIN 6
#define BUTTON 5
#define PIEZO 7
#define NUMPIXELS 8
#define OLED_MOSI  9
#define OLED_CLK   10
#define OLED_DC    11
#define OLED_CS    12
#define OLED_RESET 13

long timeThis, timeLast, timeLast2; // Millis

int idle;

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT,
                         OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

void setup() {

  pinMode(ANIM1, OUTPUT);
  pinMode(ANIM2, OUTPUT);
  pinMode(ANIMJUMP, OUTPUT);
  pinMode(PIN, OUTPUT);
  pinMode(BUTTON, INPUT);
  pinMode(PIEZO, OUTPUT);
  pixels.begin();
  for (int i = 0; i < 8; i++) {
    pixels.setPixelColor(i, pixels.Color(0, 150, 0)); // LED strip off
  }
  pixels.show();
  display.begin(SSD1306_SWITCHCAPVCC);
  Serial.begin(9600);


  display.clearDisplay();
  display.display();
}

void loop() {
  timeThis = millis();
  if ( digitalRead(BUTTON) == HIGH) {
    Serial.println("yup");
    tone(7, 440, 100);
    digitalWrite(ANIM1, LOW);
    digitalWrite(ANIM2, LOW);
    digitalWrite(ANIMJUMP, LOW);
    delay(500);
  }
  idleAnim();
  digitalWrite(ANIM1, HIGH);
  digitalWrite(ANIM2, HIGH);
  digitalWrite(ANIMJUMP, HIGH);
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
    timeLast2 = timeThis;
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
  display.print("High:"); display.println(4);
  display.display();
  display.startscrollleft(0x00, 0x0F);
}
