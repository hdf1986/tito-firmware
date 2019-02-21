
#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServerSecure.h>

IPAddress apIP(192, 168, 1, 1);
DNSServer dnsServer;
// ESP8266WebServer webServer(80);

ESP8266WebServerSecure webServer(443);

static const uint8_t rsakey[] PROGMEM = {
  #include "key.h"
};

static const uint8_t x509[] PROGMEM = {
  #include "x509.h"
};

void setup() {
  pinMode(D0, OUTPUT);
  pinMode(D1, OUTPUT);
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  WiFi.softAP("Tito");

  webServer.setServerKeyAndCert_P(rsakey, sizeof(rsakey), x509, sizeof(x509));

  // replay to all requests with same HTML
  webServer.on("/up", []() {
    digitalWrite(D0, HIGH);
    digitalWrite(D1, HIGH);
    webServer.send(200, "text/plain", "ok");
  });

  webServer.on("/right", []() {
    digitalWrite(D0, LOW);
    digitalWrite(D1, HIGH);
    webServer.send(200, "text/plain", "ok");
  });

  webServer.on("/left", []() {
    digitalWrite(D0, HIGH);
    digitalWrite(D1, LOW);
    webServer.send(200, "text/plain", "ok");
  });

  webServer.on("/stop", []() {
    digitalWrite(D0, LOW);
    digitalWrite(D1, LOW);
    webServer.send(200, "text/plain", "ok");
  });
  webServer.begin();


  digitalWrite(D0, LOW);
  digitalWrite(D1, LOW);
}

void loop() {
  webServer.handleClient();
}
