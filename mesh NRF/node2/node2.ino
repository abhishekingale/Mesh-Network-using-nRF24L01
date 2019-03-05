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
const byte rxAddr[6] = "00001";
const byte txAddrn2[6]="00002";
const byte txAddrn3[6] = "00003";
unsigned long lastTime = 0;
byte nodeid='2';
char text[32];
char text1[32];
int i=0;
int x;
bool ok;

void setup()
{
  Serial.begin(9600);
  radio.begin();
  sensors.begin();
  pinMode(5,OUTPUT);
  pinMode(2,OUTPUT);
  
  
}

void loop(){  
    Serial.println("Starting void loop");
    i=0;
    receiveData();
    if(ok){
        Serial.println("ok...\n\r");
        digitalWrite(5,HIGH);
        delay(50);
        digitalWrite(5,LOW);
        ok=false;
        }
    delay(50);
    delay(50);
    Serial.println("step2");
    if ((millis() - lastTime) >=5000){
      Serial.println("inside if millis() condition");
      getData();
      delay(50);
      text[1]= char(x);
      text[0]=nodeid;
      Serial.println(text);
      i=0;
      sendData();
      lastTime = millis();
      }
      if(ok){
        Serial.println("ok...\n\r");
        digitalWrite(5,HIGH);
        delay(50);
        digitalWrite(5,LOW);
        ok=false;
        }
                                }

    void getData()
{     Serial.println("getting data");
      sensors.requestTemperatures();
      x=sensors.getTempCByIndex(0);
      Serial.println(x);
}
void sendData(){
  Serial.println("sending data");
    radio.setRetries(15, 15);
    Serial.println("radio set");
    radio.openWritingPipe(txAddrn2);
    Serial.println("pipe set");
    radio.stopListening();
    Serial.println("listening stopped");
    ok=false;
    while(!ok){
    ok = radio.write(&text, sizeof(text));
      Serial.println("radio written");  
      
      if (!ok) {
          Serial.println("failed.\n\r");
          delay(100);
        
         }
      i++;
      if(i==25)
         {  Serial.println("tried 15 times,now switching to node 3");
            radio.setRetries(15, 15);
            Serial.println("radio set");
            radio.openWritingPipe(txAddrn3);
            Serial.println("pipe set");
            radio.stopListening();
            }
         if(i>=50)
         {break;}  
     Serial.println(i);    
    }
}
void receiveData(){
  radio.setRetries(15, 15);
  radio.openReadingPipe(0, rxAddr);
  radio.startListening();
  Serial.println("listening");
  delay(50);
  if (radio.available())
  {
    
    radio.read(&text1, sizeof(text1));
    Serial.println(text1);
    
    digitalWrite(2,HIGH);
    delay(50);
    digitalWrite(2,LOW);
    for(int j=0; j<32;j++)
    text[j]=text1[j];
    sendData();
    }
  
  
}

