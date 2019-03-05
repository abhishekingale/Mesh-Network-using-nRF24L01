#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// Data wire is plugged into port 2 on the Arduino
#define ONE_WIRE_BUS 3

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);

RF24 radio(8, 9);

const byte rxAddr[6] = "00002";
const byte txAddr[6] = "00003";
char text2[32];
char text3[32];
byte nodeid='3';
unsigned long lastTime=0;

void setup()
{
  while (!Serial);
  Serial.begin(9600);
  sensors.begin();
  radio.begin();
  pinMode(2,OUTPUT);
  pinMode(5,OUTPUT);

}

void loop()
{
  digitalWrite(2,LOW);
  digitalWrite(5,LOW);  
  relay();
if ((millis() - lastTime) > 5000){
   create_data();
   lastTime = millis();
  
}
}
  void relay(){
  radio.setRetries(15, 15);
  radio.openReadingPipe(0, rxAddr);
  radio.startListening();
  if (radio.available())
  {
    
    radio.read(&text2, sizeof(text2));
    Serial.println(text2);
    
    digitalWrite(2,HIGH);
    delay(50);
    digitalWrite(2,LOW);
    radio.setRetries(15, 15);
    radio.openWritingPipe(txAddr);
    radio.stopListening();
    bool ok = false;
    while (!ok) {
      ok = radio.write(&text2, sizeof(text2));
      if (!ok) {
        Serial.println("failed.\n\r");
        delay(50);
        }
    }
    Serial.println("ok...\n\r");
    digitalWrite(5,HIGH);
    delay(50);
    digitalWrite(5,LOW);
    } 
  else
  Serial.println("disconnected");
  
  
  }
 
  void create_data(){
    sensors.requestTemperatures();
    int x=sensors.getTempCByIndex(0);
    Serial.println(x);
    text3[1]= char(x);
    text3[0]=nodeid;
    radio.setRetries(15, 15);
    radio.openWritingPipe(txAddr);
    radio.stopListening();
    
    
    bool ok = false;
    while (!ok) {
      ok = radio.write(&text3, sizeof(text3));
      if (!ok) {
        Serial.println("failed.\n\r");
        delay(50);
        }
    }
    Serial.println("ok...\n\r");
    digitalWrite(5,HIGH);
    delay(50);
    digitalWrite(5,LOW);
       
  }

