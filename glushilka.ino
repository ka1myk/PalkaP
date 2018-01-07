/*
данный скетч имитирует глушилку
*/

void setup() { 
pinMode(3, OUTPUT); 
}

void loop() {
digitalWrite(3, HIGH);
delayMicroseconds(500);
digitalWrite(3, LOW);
delayMicroseconds(500);
}
