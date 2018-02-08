/*
Данный скетч принимает код с брелока NICE FLOR-S
*/

#define rxPin 2 //пин приемника

unsigned long p_len, p_len_prev;
volatile byte level=255;
volatile unsigned long last, len;
byte p_level;
   
 byte state = 0;
 byte i = 0; // count of bits stored
 byte R0, R1, R2, R3, R4, R5, R6, R7, R8, R9, R10, R11, R12;  
 byte nicereceive[52];
 
void setup() {
  
 Serial.begin(9600); //скорость у UNO 9600
 Serial.println("NICE LOGGER"); 
 pinMode(rxPin, INPUT);
 attachInterrupt(0, rx_int, CHANGE); //первый аргумент для UNO - 0
 interrupts();

}

void loop()
{ 
  if(level!=255)
  {
    noInterrupts();
    p_level=level;
    p_len=len;
    len=0;
    level=255;
    interrupts();    
    flors();
    p_len_prev=p_len;
  }
  
  if (state == 100) 
  {

    R0 = 0;
  for(int m=0;m<4;m++){
    if (nicereceive[m] == 0) {  R0 = (R0 << 1) | 0; } else   { R0 = (R0 << 1) | 1; }
    R1 = 0;
   }
   for(int m=4;m<8;m++){
    if (nicereceive[m] == 0) {  R1 = (R1 << 1) | 0; } else   { R1 = (R1 << 1) | 1; }
    R2 = 0;
   }
   for(int m=8;m<12;m++){  
    if (nicereceive[m] == 0) {  R2 = (R2 << 1) | 0; } else   { R2 = (R2 << 1) | 1; }
    R3 = 0;
   }
    for(int m=12;m<16;m++){  
    if (nicereceive[m] == 0) {  R3 = (R3 << 1) | 0; } else   { R3 = (R3 << 1) | 1; }
    R4 = 0;
   }
    for(int m=16;m<20;m++){  
    if (nicereceive[m] == 0) {  R4 = (R4 << 1) | 0; } else   { R4 = (R4 << 1) | 1; }
    R5 = 0;
   }
   for(int m=20;m<24;m++){  
    if (nicereceive[m] == 0) {  R5 = (R5 << 1) | 0; } else   { R5 = (R5 << 1) | 1; }
   R6 = 0;
   }
   for(int m=24;m<28;m++){  
    if (nicereceive[m] == 0) {  R6 = (R6 << 1) | 0; } else   { R6 = (R6 << 1) | 1; }
    R7 = 0;
   }
   for(int m=28;m<32;m++){  
    if (nicereceive[m] == 0) {  R7 = (R7 << 1) | 0; } else   { R7 = (R7 << 1) | 1; }
    R8 = 0;
   }
   for(int m=32;m<36;m++){  
    if (nicereceive[m] == 0) {  R8 = (R8 << 1) | 0; } else   { R8 = (R8 << 1) | 1; }
    R9 = 0;
   }
   for(int m=36;m<40;m++){  
    if (nicereceive[m] == 0) {  R9 = (R9 << 1) | 0; } else   { R9 = (R9 << 1) | 1; }
    R10 = 0;
   }
   for(int m=40;m<44;m++){  
    if (nicereceive[m] == 0) {  R10 = (R10 << 1) | 0; } else   { R10 = (R10 << 1) | 1; }
    R11 = 0;
   }
   for(int m=44;m<48;m++){  
    if (nicereceive[m] == 0) {  R11 = (R11 << 1) | 0; } else   { R11 = (R11 << 1) | 1; }
    R12 = 0;
   }
   for(int m=48;m<52;m++){  
    if (nicereceive[m] == 0) {  R12 = (R12 << 1) | 0; } else   { R12 = (R12 << 1) | 1; }
   }
 if (((R0 == 0x1) && ((R1 == 0xE) || (R1 == 0xF) || (R1 == 0xC) || (R1 == 0xD) || (R1 == 0xA) || (R1 == 0xB) || (R1 == 0x8) ||  (R1 == 0x9) || (R1 == 0x6))) ||  
    ((R0 == 0x2) && ((R1 == 0xD) || (R1 == 0xC) || (R1 == 0xF) || (R1 == 0xE) || (R1 == 0x9) || (R1 == 0x8) || (R1 == 0xB) ||  (R1 == 0xA) || (R1 == 0x5))))   { 
 Serial.print("NICE FLOR-S: ");
 Serial.print(R0 , HEX); 
 Serial.print(R1 , HEX); 
 Serial.print(R2 , HEX); 
 Serial.print(R3 , HEX);
 Serial.print(R4 , HEX); 
 Serial.print(R5 , HEX);
 Serial.print(R6 , HEX);
 Serial.print(R7 , HEX);
 Serial.print(R8 , HEX);
 Serial.print(R9 , HEX);
 Serial.print(R10, HEX);
 Serial.print(R11, HEX);
 Serial.print(R12, HEX);
 Serial.println(""); 
 }
 
 state=0;
  }
}

void rx_int()
{
  if(level!=255) return;
  
  len=micros()-last;
  last=micros();
  
  if(digitalRead(rxPin)==HIGH) level=0;
    else level=1; 
} 
 
void flors() {

switch(state) 
 {
 case 0:
 
      if(p_level) break;
      state=1;
      i=0;
      for(int m=0;m<52;m++){ nicereceive[m] = 0; }
      break;
       
  case 1:
       
      if (((p_len > 1400) && (p_len < 1600)) && (p_level))  {
           state = 2;         
           break; 
      }
        else
      {
       state = 0;
       break;     
      }
      break;
     
 case 2:

  if(!p_level) break; 
  
    if ((p_len > 900) && (p_len < 1100) )  // valid 1
    {
      nicereceive[i] = (nicereceive[i] << 1) | 1; 
      i++;
    }
    else if  ((p_len > 400) && (p_len < 600) )  // valid 0
    {
      nicereceive[i] = (nicereceive[i] << 1) | 0;
      i++;
    }
    if  (i == 52) state = 100;
    break;
    
   }  
}
