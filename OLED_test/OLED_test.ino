//test change
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_MOSI   9
#define OLED_CLK   10
#define OLED_DC    11
#define OLED_CS    12
#define OLED_RESET 13
Adafruit_SSD1306 display(OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);

void setup()   {

  display.begin(SSD1306_SWITCHCAPVCC);
  display.clearDisplay();
  display.drawCircle(display.width() / 2, display.height() / 2, 15, WHITE);
  display.drawCircle(display.width() / 2, display.height() / 2, 8, WHITE); 
  display.drawPixel(64, 32, WHITE); 
  display.display();
}

void loop() {

}

