#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char *ssid = "ESP8266-AP";
const char *password = "123456789";
ESP8266WebServer server(80);

void displayWebsite() {
  WiFi.softAP(ssid, password);
  IPAddress IP = WiFi.softAPIP();
  Serial.print("Adresa IP a AP-ului: ");
  Serial.println(IP);

  server.on("/", HTTP_GET, []() {
    String html = "<html><body><h1>Pill Dispenser</h1>";
    html += "<form action='/' method='post'><input type='submit' value='Rotate Servo'></form>";
    html += "</body></html>";
    server.send(200, "text/html", html);
  });

  server.on("/", HTTP_POST, []() {
    handleButtonPress();
    String html = "<html><body><h1>Hello</h1>";
    html += "<form action='/' method='post'><input type='submit' value='Rotate Servo'></form>";
    html += "</body></html>";
    server.send(200, "text/html", html);  });
}

void handleButtonPress() {
  digitalWrite(5, HIGH);
  delay(1000);
  digitalWrite(5, LOW);
}

void setup() {
  Serial.begin(115200);
  pinMode(5, OUTPUT);
  displayWebsite();
  server.begin();
}

void loop() {
  server.handleClient();
}
