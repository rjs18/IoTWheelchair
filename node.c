#include <SoftwareSerial.h>
#include <ThingSpeak.h>
#include <ESP8266WiFi.h>

WiFiClient  client;
unsigned long counterChannelNumber = 1216892;           // Channel ID
const char * myCounterReadAPIKey = "9Z82TZV1HPSRADZ5";  // Read API Key
const char * myCounterWriteAPIKey = "7KT6BTUKLMTISS4A"; // Write API Key

SoftwareSerial s(D6, D5);
int status_;

void setup()
{
  Serial.begin(115200);
  s.begin(115200);
  Serial.println();

  WiFi.begin("WIFI name", "WIFI password");

  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());
  ThingSpeak.begin(client);
}

void loop() 
{
  sensorOldValue = sensorNewValue;
  sensorNewValue = analogRead(analogPin);
   if(s.available()>0){
        status_ = s.read();
        Serial.print(status_);
        Serial.print("\t time : ");
        Serial.println(millis());
        ThingSpeak.writeField(counterChannelNumber , 1 , status_ , myCounterWriteAPIKey);
    }
    else{
      Serial.println("Status : Normal");
      ThingSpeak.writeField(counterChannelNumber , 1 , 0 , myCounterWriteAPIKey);
    }
    delay(50);
}
