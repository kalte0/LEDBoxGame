
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
#define LINES_HEIGHT 30
#define LETTER_HEIGHT 5

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


int y;
int b;
int space = SPACE_1;
int nick1 = 0x40;
int nick2 = 0x41;
int nick3 = 0x41;
int whichNick = 1;


void setup() {
  Serial.begin(9600);
  pinMode(5, INPUT);
  display.begin();
  display.clearDisplay();
  line_on(SPACE_1);
  line_on(SPACE_2);
  line_on(SPACE_3);
  display.display();
}


void loop() {
  timeThis = millis();
  y = key_read(5);

  if (timeThis - timeLast > 600) {
    if (b == 1) b = 0;
    else if (b == 0) b = 1; // there's gotta be a better way to do this toggle.
    timeLast = timeThis;
  }

  if (y == KEY_SHORT_PRESS) {
    if (space == SPACE_1) addNick(nick1);
    if (space == SPACE_2) addNick(nick2);
    if (space == SPACE_3) addNick(nick3);
    for (int i = 0; i < 15; i++) {
      display.fillRect(space + i, LETTER_HEIGHT + i / 2, space - i, LETTER_HEIGHT - i / 2, BLACK);
      display.display();
      delay(1); 
    }
    display.setTextSize(3);
    display.setTextColor(WHITE);
    display.setCursor(SPACE_1 + 8, LETTER_HEIGHT);
    display.print((char)nick1);
    display.setCursor(SPACE_2 + 8, LETTER_HEIGHT);
    display.print((char)nick2);
    display.setCursor(SPACE_3 + 8, LETTER_HEIGHT);
    display.print((char)nick3);
    display.display();
  }
  if (y == KEY_LONG_PRESS) {
    if (space == SPACE_1) space = SPACE_2;
    else if (space == SPACE_2) space = SPACE_3;
    else if (space == SPACE_3) space = SPACE_1;
    Serial.print("long");
  }

  if (b == 0) {
    lineAll();
    display.drawLine(0, LINES_HEIGHT - 3, 128, LINES_HEIGHT - 3, BLACK);
    Serial.println("b=0");
    display.display();
  }
  else if (b == 1) {
    lineAll();
    display.drawLine(space, LINES_HEIGHT - 3, space + 30, LINES_HEIGHT - 3, WHITE);
    Serial.println("b=1");
    display.display();
  }



}

int addNick(int nick) {
  nick++;
  if (nick == 0x5B) nick = 0x41;
  // Serial.write(nick);
  if (space == SPACE_1) nick1 = nick;
  if (space == SPACE_2) nick2 = nick;
  if (space == SPACE_3) nick3 = nick;

}
int lineAll() {
  line_on(SPACE_1);
  line_on(SPACE_2);
  line_on(SPACE_3);
}






