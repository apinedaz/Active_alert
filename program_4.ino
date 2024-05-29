#include <ESP8266WebServer.h>

#include <Wire.h>

#include <PN532_I2C.h>

#include <PN532.h>

#include <NfcAdapter.h>

PN532_I2C pn532_i2c (Wire) ;

NfcAdapter nfc = NfcAdapter (pn532_i2c);

String tag_on = "03 96 DE B6";

String tag_off_1 = "53 B7 DA 0E";

String tag_off_2 = "04 CB D5 DA AB 5C 80";

String tag_off_3 = "04 7C D4 DA AB 5C 80";

String tag_off_4 = "04 80 D4 DA AB 5C 80";

# define LED  D3
# define verify1 D5
# define verify2 D6
# define verify3 D7
# define verify4 D8

const char* ssid = "vivo Y33s";
const char* password = "pipe0102";

ESP8266WebServer server(80);

void handleRoot() {
  server.send(200, "text/plain", "Hola desde ESP8266!");
}

void handlePost() {
  digitalWrite(LED, HIGH);
  String message = server.arg("message");
  Serial.println("Mensaje recibido: " + message);
  server.send(200, "text/plain", "Mensaje recibido correctamente.");
}

void off() {
  digitalWrite(LED, LOW);
  String message = server.arg("message");
  Serial.println("Mensaje recibido: " + message);
  server.send(200, "text/plain", "Mensaje recibido correctamente.");
}

void setup() {
  pinMode(LED, OUTPUT); 
  pinMode(verify1, OUTPUT);
  pinMode(verify2, OUTPUT);
  pinMode(verify3, OUTPUT);
  pinMode(verify4, OUTPUT);

  digitalWrite(LED, LOW);
  digitalWrite(verify1, LOW);
  digitalWrite(verify2, LOW);
  digitalWrite(verify3, LOW);
  digitalWrite(verify4, LOW);

  Serial.begin(115200);

  nfc.begin ();
  
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando al WiFi...");
  }

  Serial.println("Conectado al WiFi!");
  Serial.println("Dirección IP:");
   Serial.println(WiFi.localIP());
  server.on("/", HTTP_GET, handleRoot);
  server.on("/on", HTTP_POST, handlePost);
  server.on("/off", HTTP_POST, off);
  server.begin();
  Serial.println("Servidor HTTP iniciado.");
}

void loop() {
  server.handleClient();
  if (nfc.tagPresent ())
  {
    NfcTag tag = nfc.read ();
    if(tag.getUidString() == tag_on){
      digitalWrite(LED, HIGH);
    }
    if(tag.getUidString() == tag_off_1 && digitalRead(verify2) == 0 && digitalRead(verify3) == 0 && digitalRead(verify4) == 0 && digitalRead(LED) == 1){
      digitalWrite(verify1, HIGH);
    }
    if(tag.getUidString() == tag_off_2 && digitalRead(verify1) == 1){
      digitalWrite(verify2, HIGH);
    }
    if(tag.getUidString() == tag_off_3 && digitalRead(verify2) == 1){
      digitalWrite(verify3, HIGH);
    }
    if(tag.getUidString() == tag_off_4 && digitalRead(verify3) == 1){
      digitalWrite(verify4, HIGH);
    }
    if(digitalRead(verify4))
    {
      delay(1000);
      digitalWrite(LED, LOW);
      digitalWrite(verify1, LOW);
      digitalWrite(verify2, LOW);
      digitalWrite(verify3, LOW);
      digitalWrite(verify4, LOW);
    }
  }
}