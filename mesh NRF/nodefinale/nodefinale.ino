#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(8, 9);

const byte rxAddr[6] = "00003";

void setup()
{
  while (!Serial);
  Serial.begin(9600);
  Serial1.begin(9600);
  Serial.println("Starting...");
  
  radio.begin();
  radio.openReadingPipe(0, rxAddr);
  
  radio.startListening();
}

void loop()
{
  if (radio.available())
  {
    char text[2] = {0};
    radio.read(&text, sizeof(text));
    
    Serial.println(text[0]);
    Serial.println((int)text[1]);
    Serial1.write(text);

  }
}
