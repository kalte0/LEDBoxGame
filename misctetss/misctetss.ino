
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
int nick2 = 0x40;
int nick3 = 0x40;

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
  y = key_read(5);

  if (y == KEY_SHORT_PRESS) {
    writeNick(nick1, space);
  }
  if (y == KEY_LONG_PRESS){ 
  Serial.print("long");
  }
  
  delay(10);


}

int writeNick(int nick, int SPACE) {
  nick++;
  if (nick == 0x5B) nick = 0x41;
  Serial.write(nick);
  display.clearDisplay();
  display.setCursor(SPACE + 8, LETTER_HEIGHT);
  display.setTextSize(3);
  display.setTextColor(WHITE);
  display.print((char)nick);
  line_on(SPACE_1);
  line_on(SPACE_2);
  line_on(SPACE_3);
  display.display();
  if (space == SPACE_1) nick1 = nick;
  if (space == SPACE_2) nick2 = nick; 
  if (space == SPACE_3) nick3 = nick; 
 
}




