#include "DHT.h"
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#define PIN_RELAY1 4
#define PIN_RELAY2 5
#define DHTPIN 2

const char* ssid = "inet";
const char* password = "18811881";
const char* mqtt_server = "192.168.1.13"; 
const char* mqtt_user = "username";
const char* mqtt_pass= "password";
const uint8_t  pinSensor = A0;      

WiFiClient espClient;
PubSubClient client(espClient);
DHT dht(DHTPIN, DHT22);

void setup_wifi() {
  // Connecting to a WiFi network
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  // Loop until we're reconnected
  Serial.println("In reconnect...");
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("Arduino_Gas", mqtt_user, mqtt_pass)) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void setup() {
  pinMode(PIN_RELAY1, OUTPUT); 
  digitalWrite(PIN_RELAY1, HIGH); 
  pinMode(PIN_RELAY2, OUTPUT); 
  digitalWrite(PIN_RELAY2, HIGH); 
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  dht.begin();
}
void loop() {
  delay(1000);
char tmp[8];
  if (!client.connected()) {
    reconnect();
  }
  char hmd[8];
  if (!client.connected()) {
    reconnect();
  }  
float humidityDHT22 = dht.readHumidity(); 
float temperatureDHT22 = dht.readTemperature();   
 
  if (isnan(humidityDHT22) || isnan(temperatureDHT22)) {  
    Serial.println("Error read");
    return;
  }  
  if (humidityDHT22 >= 60.00 && humidityDHT22 <= 65.00){
digitalWrite(PIN_RELAY1, LOW); 
}
  else if (humidityDHT22 < 60.00 || humidityDHT22 > 65.00){
digitalWrite(PIN_RELAY1, HIGH);    
}
  if (humidityDHT22 >= 70.00 && humidityDHT22 <= 75.00){
digitalWrite(PIN_RELAY2, LOW); 
}
  else if (humidityDHT22 < 70.00 || humidityDHT22 > 75.00 ){
digitalWrite(PIN_RELAY2, HIGH);    
}
Serial.print("Humiditty: ");
Serial.print(humidityDHT22);
Serial.print(" %\t");
Serial.print("Temperature: ");
Serial.print(temperatureDHT22);
Serial.println(" *C "); 
Serial.println(analogRead(pinSensor));


}
