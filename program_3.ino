#include <ESP8266WebServer.h>

#include <Wire.h>

#include <PN532_I2C.h>

#include <PN532.h>

#include <NfcAdapter.h>

PN532_I2C pn532_i2c (Wire) ;

NfcAdapter nfc = NfcAdapter (pn532_i2c);

String tag_on = "03 96 DE B6";

String tag_off_1 = "53 B7 DA 0E";

String tag_off_2 = "04 80 D4 DA AB 5C 80";


String tag_off_2 = "04 80 D4 DA AB 5C 80";

# define LED  D3

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
  digitalWrite(LED, LOW);
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
  server.on("/post", HTTP_POST, handlePost);
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
    if(tag.getUidString() == tag_off){
      digitalWrite(LED, LOW);
    }
  }
}
