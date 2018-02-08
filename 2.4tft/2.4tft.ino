#include <SPFD5408_Adafruit_GFX.h>    // Core graphics library
#include <SPFD5408_Adafruit_TFTLCD.h> // Hardware-specific library
#include <SPFD5408_TouchScreen.h>

#define LCD_CS A3 // Chip Select goes to Analog 3
#define LCD_CD A2 // Command/Data goes to Analog 2
#define LCD_WR A1 // LCD Write goes to Analog 1
#define LCD_RD A0 // LCD Read goes to Analog 0
#define LCD_RESET A4 // Can alternately just connect to Arduino's reset pin

#define	BLACK   0x0000
#define	BLUE    0x001F
#define	RED     0xF800
#define	GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);

void setup(void) {
  
  Serial.begin(9600);
  tft.reset();

  uint16_t identifier = tft.readID();

  tft.begin(identifier);
  
  tft.begin(0x8357); // SDFP5408

  tft.setRotation(0); // Need for the Mega, please changed for your choice or rotation initial


  Serial.println(testText());
  delay(3000);

}

void loop(void) {

}

unsigned long testText() {
  tft.fillScreen(BLACK);
  tft.setCursor(0, 0);
  tft.println("1");
  tft.println("2");
  tft.println("3");
  tft.println("4");
  tft.println("5");
  tft.println("6");
  tft.println("7");
  tft.println("8");
  tft.println("9");
  tft.println("10");
  tft.println("11");
  tft.println("12");
  tft.println("13");
  tft.println("14");
  tft.println("15");
  tft.println("16");
  tft.println("17");
  tft.println("18");
  tft.println("19");
  tft.println("20");
  
}


