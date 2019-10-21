
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>



#define KEY_PRESSED HIGH //state of key being pressed
#define KEY_NO_PRESS 0 //key not pressed
#define KEY_SHORT_PRESS 1 //key pressed short
#define KEY_LONG_PRESS 2 //key pressed long
#define KEY_DURATION 500 //cycle count, minimum threshold to test for long presses

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#define LINES_HEIGHT 20

#define SPACE_1 10
#define SPACE_2 49
#define SPACE_3 88

#define OLED_MOSI  9
#define OLED_CLK   10
#define OLED_DC    11
#define OLED_CS    12
#define OLED_RESET 13
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT,
                         OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);

#define line_on(space)  display.drawLine(space, LINES_HEIGHT, space + 30, LINES_HEIGHT, WHITE);
#define line_off(space) display.drawLine(space, LINES_HEIGHT, space + 30, LINES_HEIGHT, BLACK);

long timeThis, timeLast;

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


int x = 0x40;
int y;
int b;
int space = SPACE_1;

void setup() {
  Serial.begin(9600);
  pinMode(5, INPUT);
  display.begin();
  display.clearDisplay();
  /*  display.drawLine(10, LINES_HEIGHT, 40, LINES_HEIGHT, WHITE);
    display.drawLine(49, LINES_HEIGHT, 79, LINES_HEIGHT, WHITE);
    display.drawLine(88, LINES_HEIGHT, 118, LINES_HEIGHT, WHITE);*/
  line_on(SPACE_1);
  line_on(SPACE_2);
  line_on(SPACE_3);
  display.display();
}

void loop() {
  if (timeThis - timeLast > 500) {
    if (b == 0) {
      line_on(space);
      b = 1;
    }
    else if (b == 1) {
      line_off(space);
      b = 0;
    }
  }
  y = key_read(5);
  if (y == KEY_SHORT_PRESS) {
    x++;
    if (x == 0x5B) x = 0x41;
    Serial.write(x);
  }
  if (y == KEY_LONG_PRESS) Serial.print("long");
  delay(10);


}

