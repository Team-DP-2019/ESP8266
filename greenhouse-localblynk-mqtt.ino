#include "DHT.h"
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <BlynkSimpleEsp8266.h>

const uint8_t soilPin = A0;
#define BLYNK_PRINT Serial
#define DHTPIN 2
#define BUFFER_SIZE 100
#define RELAY1 4
#define RELAY2 5
#define RELAY_SIGNALING 0
#define RELAY_HEATING 16

char auth[] = "myToken";
const char* ssid = "myWi-FI";
char pass[] = "myPass";
const char* password = "myPass";
const char* mqtt_server = "myAdress";
const char* mqtt_user = "myUser";
const char* mqtt_pass = "myPass";
const int mqtt_port = 1883;

int soil = 0;
int counterSoil = 0;
int counterTemperature = 0;
int counterHumidity = 0;
DHT dht(DHTPIN, DHT22);
WiFiClient wclient;
PubSubClient client(wclient, mqtt_server, mqtt_port);
BlynkTimer timer;

void myTimerEvent()
{
    //Blynk.virtualWrite(V100, t);
    //Blynk.virtualWrite(V99, h);
    //Blynk.virtualWrite(V98, soil);
}

void setup()
{
    Serial.begin(115200);
    Blynk.begin(auth, ssid, pass, IPAddress(192, 168, 1, 24), 80);
    //Blynk.begin(auth, ssid, pass);
    timer.setInterval(1000L, myTimerEvent);
    pinMode(RELAY1, OUTPUT);
    digitalWrite(RELAY1, HIGH);
    pinMode(RELAY2, OUTPUT);
    digitalWrite(RELAY2, HIGH);
    pinMode(RELAY_SIGNALING, OUTPUT);
    digitalWrite(RELAY_SIGNALING, HIGH);
    pinMode(RELAY_HEATING, OUTPUT);
    digitalWrite(RELAY_HEATING, HIGH);
    dht.begin();
}

void loop()
{
    delay(1000);
    int soil = analogRead(soilPin);
    Blynk.run();
    timer.run();
    float h = dht.readHumidity();
    float t = dht.readTemperature();
    String temerature = "Сritical temerature: ";
    String temeratureError = temerature + t;
    String humidity = "Сritical humidity: ";
    String humidityError = humidity + h;
    Blynk.virtualWrite(V100, t);
    Blynk.virtualWrite(V99, h);
    Blynk.virtualWrite(V98, soil);
    if (isnan(h) || isnan(t)) {
        Serial.println("Read error DHT22!");
        return;
    }

    if (WiFi.status() != WL_CONNECTED) {
        Serial.print("Connecting to ");
        Serial.print(ssid);
        Serial.println("...");
        WiFi.begin(ssid, pass);

        if (WiFi.waitForConnectResult() != WL_CONNECTED)
            return;
        Serial.println("WiFi connected");
    }

    // подключаемся к MQTT серверу
    if (WiFi.status() == WL_CONNECTED) {
        if (!client.connected()) {
            Serial.println("Connecting to MQTT server");
            if (client.connect(MQTT::Connect("arduinoClient2")
                                   .set_auth(mqtt_user, mqtt_pass))) {
                Serial.println("Connected to MQTT server");
                client.subscribe("iot_data");
            }
            else {
                Serial.println("Could not connect to MQTT server");
            }
        }

        if (client.connected()) {
            client.loop();
            client.publish("iot_data", "{id:1,type:1," + String(t) + "}");
            client.publish("iot_data", "{id:2,type:2," + String(h) + "}");
            client.publish("iot_data", "{id:3,type:2," + String(soil) + "}");
        }
    }

    if (t < 10 || t > 35 || isnan(t)) {
        Blynk.email("dshpval@gmail.com", "Greenhouse ALARM!", temeratureError);
    }

    if (t < 10 || t > 35 || isnan(t)) {
        counterTemperature++;
        if (counterTemperature == 200) {
            Blynk.email("dshpval@gmail.com", "Greenhouse ALARM!", temeratureError);
            counterTemperature = 0;
        }
    }
    if (h < 10 || t > 80 || isnan(h)) {
        counterHumidity++;
        if (counterHumidity == 200) {
            Blynk.email("dshpval@gmail.com", "Greenhouse ALARM!", humidityError);
            counterHumidity = 0;
        }
    }
    

    
    if (t >= 20 && t <= 25) {
        digitalWrite(RELAY1, LOW);
    }
    else if (t < 20 || t <= 25) {
        digitalWrite(RELAY1, HIGH);
    }
    else if (t >= 26 && t <= 27) {
        digitalWrite(RELAY2, LOW);
    }
    else if (t < 26 || t) {
        digitalWrite(RELAY2, HIGH);
    }
    Serial.println(soil);
    if (soil > 900) {
        counterSoil++;
        if (counterSoil == 200) {
            Blynk.email("dshpval@gmail.com", "Greenhouse ALARM!", "Dry Soil!");
            counterSoil = 0;
        }
    }

else if (soil < 150){
    counterSoil++;
    if (counterSoil == 200) {
        Blynk.email("dshpval@gmail.com", "Greenhouse ALARM!", "Flood!");
        counterSoil = 0;
    }
}
 

Serial.print("hum: ");
Serial.print(h);
Serial.print(" %\t");
Serial.print("temp: ");
Serial.print(t);
Serial.println(" *C ");
}
       
    
     
