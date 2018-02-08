#include <Keypad.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display


const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
//define the cymbols on the buttons of the keypads
char hexaKeys[ROWS][COLS] = {
  {'P','O','N','M'},
  {'L','K','J','I'},
  {'H','G','F','E'},
  {'D','C','B','A'}
};
byte rowPins[ROWS] = {4, 6, 2, 3}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {7, 8, 9, 10 }; //connect to the column pinouts of the keypad

//initialize an instance of class NewKeypad
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

void setup(){
  lcd.init();                      // initialize the lcd 
  lcd.backlight();
  Serial.begin(9600);
}
  
void loop(){
  char customKey = customKeypad.getKey();
  
  if (customKey){
    lcd.write(customKey);
    delay(300);
    lcd.clear();
  }
}
