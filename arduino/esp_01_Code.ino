#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

// Define default AP credentials if not already defined
#ifndef APSSID
#define APSSID "ESPap"
#define APPSK ""
#endif

const char* ssid = APSSID;
const char* password = APPSK;

ESP8266WebServer server(80);

void handleRoot() {

  if (server.hasArg("cmd")) {
    String cmd = server.arg("cmd");

    Serial.println(cmd);

    server.send(200, "text/html",
      "<h1>Command received: " + cmd + "</h1>");
  }
  else {
    server.send(200, "text/html",
      "<h1>No command sent</h1>");
  }
}

void setup() {
    Serial.begin(9600);
    delay(1000);
    Serial.println("\nConfiguring access point...");

    WiFi.softAP(ssid, password);

    IPAddress myIP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(myIP);

    server.on("/", handleRoot);

    server.begin();
    Serial.println("HTTP server started");
}

void loop() {
    server.handleClient();
    delay(500); 
}