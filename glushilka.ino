/*
данный скетч имитирует глушилку
*/

#define txPin 3 //пин передатчика

void setup() { 
pinMode(txPin, OUTPUT); 
}

void loop() {
digitalWrite(txPin, HIGH);
delayMicroseconds(500);
digitalWrite(txPin, LOW);
delayMicroseconds(500);
}
