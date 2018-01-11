int send_code_pin = 3;
//int send_code = 3061; это код определенный методом перебора для моего шлагбаума
 
void setup()
{
  pinMode(send_code_pin, OUTPUT);
}

void loop ()
{
  for (int send_code = 0; send_code < 4096; send_code++) // этот цикл после того как код определен необходимо убрать
  {
    for (int j = 0; j <7; j++) // достаточно 4-х, но из-за помех поставил 7
    {
      digitalWrite(send_code_pin, HIGH); // стартовый импульс
      delayMicroseconds(700);
      digitalWrite(send_code_pin, LOW);
      for (int i = 12; i > 0; i--)
      {
        boolean bit_code = bitRead(send_code, i - 1);
        if (bit_code)
        {
          digitalWrite(send_code_pin, LOW); // единица
          delayMicroseconds(1400);
          digitalWrite(send_code_pin, HIGH);
          delayMicroseconds(700);
        }
        else
        {
          digitalWrite(send_code_pin, LOW); // ноль
          delayMicroseconds(700);
          digitalWrite(send_code_pin, HIGH);
          delayMicroseconds(1400);
        }
      }
      digitalWrite(send_code_pin, LOW); // пилотный период
      delayMicroseconds(25200);
    }
  }
  //delay(10000); после определения кода поставить задержку
}
