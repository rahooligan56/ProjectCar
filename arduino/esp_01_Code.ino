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


// Buffer for incoming JSON from Arduino
char jsonBuf[128];
uint8_t jsonLen = 0;
bool jsonReady = false;

// Store last valid JSON
String lastJson = "{}";


void readSerialJSON() {
  while (Serial.available()) {
    char c = Serial.read();

    if (c == '\n') {
      jsonBuf[jsonLen] = '\0';
      lastJson = jsonBuf;   // store complete JSON line
      jsonLen = 0;
      jsonReady = true;
    } else {
      if (jsonLen < sizeof(jsonBuf) - 1) {
        jsonBuf[jsonLen++] = c;
      }
    }
  }
}

void handleUltrasonic() {
  server.send(200, "application/json", lastJson);
}

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
    server.on("/ultrasonic", handleUltrasonic);

    server.begin();
    Serial.println("HTTP server started");
}

unsigned long lastYield = 0;

void loop() {
    server.handleClient();
    readSerialJSON();

    if (millis() - lastYield > 10) { // every 10ms
        yield();
        lastYield = millis();
    }
}

