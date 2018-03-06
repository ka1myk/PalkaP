//начало импорта библиотек
#include <Keypad.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
//конец импорта библиотек

LiquidCrystal_I2C lcd(0x27,20,4); //устанавливаем id экрана на l2с шине


const byte ROWS = 4; //4 строки у матричной панели
const byte COLS = 4; //4 колонны у матричной панели

//устанавливем кнопки на матричной панели
char hexaKeys[ROWS][COLS] = {
  {'P','O','N','M'},
  {'L','K','J','I'},
  {'H','G','F','E'},
  {'D','C','B','A'}
};
byte rowPins[ROWS] = {4, 6, 2, 3}; //к каким выходам подключены кнопки по строкам
byte colPins[COLS] = {7, 8, 9, 10 }; //к каким выходам подключены кнопки по столбцам

//initialize an instance of class NewKeypad
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

void setup(){
  lcd.init(); //вкл. экран 
  lcd.backlight(); //вкл. подсветлка экрана
  Serial.begin(9600);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
}
  
void loop(){
  
  char customKey = customKeypad.getKey();
  
  if (customKey=="A"){

    digitalWrite(11, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(1000);              // wait for a second
    digitalWrite(11, LOW);    // turn the LED off by making the voltage LOW
    delay(1000);              // wait for a second
    
    lcd.write(customKey);
    delay(300);
    lcd.clear();
  }

   if (customKey=="B"){

    digitalWrite(12, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(1000);              // wait for a second
    digitalWrite(12, LOW);    // turn the LED off by making the voltage LOW
    delay(1000);              // wait for a second
    
    lcd.write(customKey);
    delay(300);
    lcd.clear();
  }

  if (customKey=="C"){

    digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(1000);              // wait for a second
    digitalWrite(13, LOW);    // turn the LED off by making the voltage LOW
    delay(1000);              // wait for a second
    
    lcd.write(customKey);
    delay(300);
    lcd.clear();
  }

}
