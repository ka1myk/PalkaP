//Палкаподнималка
//Поддерживает: CAME, NICE, AM MOTORS
//Версия 2.0

#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display

#define wt 150000                   //Задержка между посылками

#define Pe 413                      //AN MOTORS
#define Pe2 Pe*2                    //AN MOTORS

#define rxPin 2                     //Приемник
#define rxOn 4                      //Включение приёмника
#define txPin 3                     //Передатчик

#define btsendPin 7                 //кнопка, ОТПРАВКА
#define btglushPin 8                //кнопка, ГЛУШИЛКА

#define ledglPin A0                 //индикатор режима глушилка
#define ledcashPin A1               //индикатор сигнала в cashcame
#define ledpwrPin A2               //индикатор режима power



//Прием
#define MAX_DELTA 200                   //Максимальное отклонение от длительности при приеме
volatile unsigned long prevtime;
volatile unsigned int lolen, hilen, state;
//AN Motors
volatile static byte bcounter = 0;      // количество принятых битов
volatile static long code1 = 0;         // зашифрованная часть
volatile static long code2 = 0;         // фиксированная часть
volatile long c1 = 0;                   // переменная для отправки
volatile long c2 = 0;                   // переменная для отправки
volatile long Cash1 = 0;                // кеш переменная для отправки
volatile long Cash2 = 0;                // кеш переменная для отправки
//Came
volatile static byte cameCounter = 0;   // хранит количество бит
volatile static long cameCode = 0;      // сам код кнопки
volatile long cashcame = 0;             // кеш переменная для отправки
//NICE
volatile static byte niceCounter = 0;   // count of bits stored
volatile static long niceCode = 0;      // code itself
volatile long cashnice = 0;             // кеш переменная для отправки

int flag = 0;                           // флаг для обработки кнопки


void setup() {

  lcd.init();
  lcd.backlight();

 
  pinMode(rxPin, INPUT);
  pinMode(rxOn, OUTPUT);
  pinMode(txPin, OUTPUT);

  pinMode(ledcashPin, OUTPUT);
  pinMode(ledglPin, OUTPUT);
  pinMode(ledpwrPin, OUTPUT);

  pinMode(btsendPin, INPUT);
  pinMode(btglushPin, INPUT);

  digitalWrite(ledpwrPin, HIGH);

  Serial.begin(9600);
  Serial.println("Scaner RUN");

  randomSeed(analogRead(0));              //Генерация рандомного числа для AN Motors

  attachInterrupt(0, grab, CHANGE);       //перехват пакетов
  digitalWrite(rxOn, HIGH);

  Serial.println(digitalRead(rxOn));
}


void loop() {
  //обработка кнопки отправки кодов
  if (digitalRead(btsendPin) == HIGH) {
    noInterrupts();                       //Выкл перехват
    digitalWrite(rxOn, LOW);
    digitalWrite(ledglPin, LOW);          //Выкл индикатор глушилки
    flag = 1;
  }
  //обработка кнопки глушилки
  if (digitalRead(btglushPin) == HIGH) {
    noInterrupts();                       //Выкл перехват
    digitalWrite(rxOn, LOW);
    digitalWrite(ledglPin, HIGH);         //Вкл индикатор глушилки
    flag = 2;
  }

  switch (flag) {
    case 1:
      anmcode();                          //AN MOTORS
      ccode();                            //CAME
      ncode();                            //NICE
      interrupts();                       //Вкл перехват
      digitalWrite(rxOn, HIGH);
      flag = 0;
      break;
    case 2:                               //Глушилка
      digitalWrite(txPin, HIGH);
      delayMicroseconds(500);
      digitalWrite(txPin, LOW);
      delayMicroseconds(499);
      break;
  }
}


// ******************************************************
//Коды AN MOTORS
void anmcode() {
  //КОД последий принятый от AN MOTORS
  if (Cash1 > 0 && Cash2 > 0) {
    c1 = Cash1 + random(0xff);
    c2 = Cash2;
    SendANMotors(c1, c2);
  }

  //Пример
  //c1 = 0x20220000 + random(0xff);
  //c2 = 0x2A3B4CE52;
  //SendANMotors(c1, c2);
}

//Коды CAME
void ccode() {
  //Отправка последнего принятого кода со сканера
  if (cashcame > 0) {
    SendCame(cashcame);
  }

  //Пример
  //SendCame(2345);
}

//Коды NICE
void ncode() {
  //Отправка последнего принятого кода со сканера
  if (cashnice > 0) {
    SendNice(cashnice);
  }

  //Пример
  //SendNice(1234);
}

// ******************************************************
//Отправка ANMotors
void SendANMotors(long c1, long c2) {
  for (int j = 0; j < 4; j++)  {
    digitalWrite(ledcashPin, !digitalRead(ledcashPin));
    //отправляем 12 начальных импульсов 0-1
    for (int i = 0; i < 12; i++) {
      delayMicroseconds(Pe);
      digitalWrite(txPin, HIGH);
      delayMicroseconds(Pe);
      digitalWrite(txPin, LOW);
    }
    delayMicroseconds(Pe * 10);
    //отправляем первую половину
    for (int i = 4 * 8; i > 0; i--) {
      SendBit(bitRead(c1, i - 1));
    }
    //вторую половину
    for (int i = 4 * 8; i > 0; i--) {
      SendBit(bitRead(c2, i - 1));
    }
    //и еще пару ненужных бит, которые означают батарейку и флаг повтора
    SendBit(1);
    SendBit(1);
    delayMicroseconds(Pe * 39);
  }
  delay(wt);
}

//Отправка ANMotors
void SendBit(byte b) {
  if (b == 0) {
    digitalWrite(txPin, HIGH);        // 0
    delayMicroseconds(Pe2);
    digitalWrite(txPin, LOW);
    delayMicroseconds(Pe);
  }
  else {
    digitalWrite(txPin, HIGH);        // 1
    delayMicroseconds(Pe);
    digitalWrite(txPin, LOW);
    delayMicroseconds(Pe2);
  }
}

//Отправка CAME
void SendCame(long Code) {
  for (int j = 0; j < 4; j++) {     // посылку посылаем несколько раз подряд.
    digitalWrite(ledcashPin, !digitalRead(ledcashPin));
    // время стартового импульса
    digitalWrite(txPin, HIGH);
    delayMicroseconds(320);
    digitalWrite(txPin, LOW);
    for (int i = 12; i > 0; i--) {
      byte b = bitRead(Code, i - 1);  // побитово перебираем и посылаем код
      if (b) {
        digitalWrite(txPin, LOW);     // 1
        delayMicroseconds(640);
        digitalWrite(txPin, HIGH);
        delayMicroseconds(320);
      }
      else {
        digitalWrite(txPin, LOW);     // 0
        delayMicroseconds(320);
        digitalWrite(txPin, HIGH);
        delayMicroseconds(640);
      }
    }
    digitalWrite(txPin, LOW);
    delayMicroseconds(11520);
  }
  delay(wt);
}

//Отправка NICE
void SendNice(long Code) {
  for (int j = 0; j < 4; j++) { // посылку посылаем 4 раза подряд.
    digitalWrite(ledcashPin, !digitalRead(ledcashPin));
    // время стартового импульса
    digitalWrite(txPin, HIGH);
    delayMicroseconds(700);
    digitalWrite(txPin, LOW);
    for (int i = 12; i > 0; i--) {
      byte b = bitRead(Code, i - 1); // побитово перебираем и посылаем код
      if (b) {
        digitalWrite(txPin, LOW); // 1
        delayMicroseconds(1400);
        digitalWrite(txPin, HIGH);
        delayMicroseconds(700);
      }
      else {
        digitalWrite(txPin, LOW); // 0
        delayMicroseconds(700);
        digitalWrite(txPin, HIGH);
        delayMicroseconds(1400);
      }
    }
    digitalWrite(txPin, LOW);
    delayMicroseconds(25200);
  }
  delay(wt);
}


// ******************************************************
//ПРИЁМ
boolean CheckValue(unsigned int base, unsigned int value) {
  return ((value == base) || ((value > base) && ((value - base) < MAX_DELTA)) || ((value < base) && ((base - value) < MAX_DELTA)));
}

void grab() {
  state = digitalRead(rxPin);
  if (state == HIGH)
    lolen = micros() - prevtime;
  else
    hilen = micros() - prevtime;
  prevtime = micros();

  //AN MOTORS
  if (state == HIGH)  {
    // последовательность закончилась
    if (CheckValue(Pe, hilen) && CheckValue(Pe2, lolen)) {        // valid 1
      if (bcounter < 32)
        code1 = (code1 << 1) | 1;
      else if (bcounter < 64)
        code2 = (code2 << 1) | 1;
      bcounter++;
    }
    else if (CheckValue(Pe2, hilen) && CheckValue(Pe, lolen)) {   // valid 0
      if (bcounter < 32)
        code1 = (code1 << 1) | 0;
      else if (bcounter < 64)
        code2 = (code2 << 1) | 0; bcounter++;
    }
    else
      bcounter = 0;
    //Вывод в терминал
    if (bcounter >= 65)  {
      lcd.setCursor(0, 0);     
      lcd.print(code1, HEX);
      lcd.setCursor(0, 1); 
      lcd.println(code2, HEX);
      digitalWrite(ledcashPin, HIGH);
      Cash1 = code1;
      Cash2 = code2;
      bcounter = 0;
      code1 = 0;
      code2 = 0;
    }
  }

  //CAME
  if (state == LOW)  {
    // последовательность закончилась
    if (CheckValue(320, hilen) && CheckValue(640, lolen)) {        // valid 1
      cameCode = (cameCode << 1) | 1;
      cameCounter++;
    }
    else if (CheckValue(640, hilen) && CheckValue(320, lolen)) {   // valid 0
      cameCode = (cameCode << 1) | 0;
      cameCounter++;
    }
    else cameCounter = 0;
  } else if (lolen > 1000 && (cameCounter == 12 || cameCounter == 13) && ((cameCode & 0xfff) != 0xfff)) {
    lcd.setCursor(0, 0); 
    lcd.print("CAME:");
    
    lcd.setCursor(0, 1);  
    lcd.println(cameCode & 0xfff);
    
    Serial.println(cameCode & 0xfff);
    cashcame = cameCode & 0xfff;
    cameCounter = 0;
    cameCode = 0;
    digitalWrite(ledcashPin, HIGH);
  }

  //NICE
  if (state == LOW)
  {
    // последовательность закончилась
    if (CheckValue(700, hilen) && CheckValue(1400, lolen)) // valid 1
    {
      niceCode = (niceCode << 1) | 1;
      niceCounter++;
    }
    else if (CheckValue(1400, hilen) && CheckValue(700, lolen)) // valid 0
    {
      niceCode = (niceCode << 1) | 0;
      niceCounter++;
    }
    else niceCounter = 0;
  } else if (lolen > 2200 && (niceCounter == 12 || niceCounter == 13) && ((niceCode & 0xfff) != 0xfff)) {
    Serial.print("NICE: ");

    lcd.setCursor(0, 0); 
    lcd.print("NICE:");
    lcd.setCursor(0, 1); 
    lcd.println(niceCode & 0xfff);
    
    Serial.println(niceCode & 0xfff);
    cashnice = niceCode & 0xfff;
    niceCounter = 0;
    niceCode = 0;
    digitalWrite(ledcashPin, HIGH);
  }

}


