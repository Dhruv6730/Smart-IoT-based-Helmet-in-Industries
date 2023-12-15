#include <WiFi.h>
#include "ThingSpeak.h"
#include <DHT.h>

const char* ssid = "Avemaria";
const char* password = "Avemaria@#";

WiFiClient  client;

unsigned long myChannelNumber = 2341175;
const char * myWriteAPIKey = "UWH7NNZUVPMR88UU";

// Pin definitions
const int gasSensorPin = 13; // Arduino MQ Gas Sensor
const int dht11Pin = 4; // DHT11 Humidity and Temperature Sensor
const int buzzerPin = 5;//Buzzer
// DHT sensor
DHT dht(dht11Pin, DHT11);

// Function declarations
float readDHT11Temperature();
int readGasSensor();
float readDHT11Humidity();

void setup(){
    Serial.begin(115200);
    delay(1000);

    WiFi.mode(WIFI_STA); //Optional
    WiFi.begin(ssid, password);
    Serial.println("\nConnecting");

    while(WiFi.status() != WL_CONNECTED){
        Serial.print(".");
        delay(100);
    }

    Serial.println("\nConnected to the WiFi network");
    Serial.print("Local ESP32 IP: ");
    Serial.println(WiFi.localIP());

    ThingSpeak.begin(client);  
// Initialize ThingSpeak

  pinMode(gasSensorPin,INPUT);
  pinMode(buzzerPin,OUTPUT);

}

void loop(){
 int x1,x2,x3;

  float temperature = readDHT11Temperature();
  int gasValue = readGasSensor();
  float humidity = readDHT11Humidity();

  Serial.print("Temperature:");
  Serial.println(temperature);
  Serial.print("Humidity:");
  Serial.println(humidity);
  Serial.print("Gas value:");
  Serial.println(gasValue); 


  if(temperature>40 || humidity>100 || gasValue>200)
   {
     digitalWrite(buzzerPin,HIGH);
   }
  else
   {
     digitalWrite(buzzerPin,LOW);
   }


  x1=ThingSpeak.writeField(myChannelNumber, 1,temperature, myWriteAPIKey);
  if(x1 == 200){
      Serial.println("Channel 1 field 1  update successful.");
    }
    else{
      Serial.println("Problem updating channel. HTTP error code " + String(x1));
    }

  delay(15000);
  x2= ThingSpeak.writeField(myChannelNumber, 2,gasValue, myWriteAPIKey);
  if(x2 == 200){
      Serial.println("Channel 1 field 2  update successful.");
    }
    else{
      Serial.println("Problem updating channel. HTTP error code " + String(x2));
    }

  delay(15000);
  x3 = ThingSpeak.writeField(myChannelNumber, 3,humidity, myWriteAPIKey);
    if(x3 == 200){
      Serial.println("Channel 1 field 3  update successful.");
    }
    else{
      Serial.println("Problem updating channel. HTTP error code " + String(x3));
    }

    delay(15000);

}

float readDHT11Temperature() {
  float rawValue = dht.readTemperature();
  return rawValue;
}

int readGasSensor() {
  int rawValue = analogRead(gasSensorPin);
  return rawValue;
}
float readDHT11Humidity() {
  float humidity = dht.readHumidity();
  return humidity;
}
