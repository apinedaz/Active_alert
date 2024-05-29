#include <ESP8266WebServer.h>

# define LED  D3

const char* ssid = "CONEXIONDIGITAL-PINEDA";
const char* password = "S4n7I_04-08";

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
}
