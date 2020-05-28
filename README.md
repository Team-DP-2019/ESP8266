# ESP8266
WeMos D1/R1 ESP8266 Arduino format code for Blynk framework and MQTT publisher

                         How to install Blynk local server on RPI

You need to have a JAR, OpenSSL

sudo git clone https://github.com/blynkkk/blynk-server/v0.41.13/

copy the following files
https://github.com/Team-DP-2019/ESP8266/blob/master/mail.properties
https://github.com/Team-DP-2019/ESP8266/blob/master/server.properties
Сonfigure your network settings

java -jar serverfile.jar -dataFolder /Path

openssl genrsa -out server_embedded.key 2048

openssl x509 -req -days 1500 -in server_embedded.csr -signkey server_embedded.key -out server_embedded.crt

openssl pkcs8 -topk8 -inform PEM -outform PEM -in server_embedded.key -out server_embedded.pem
