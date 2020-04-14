/*
    OTA update over HTTPS
    
    As an example, we download and install ESP8266Basic firmware from github.
    
    Requires latest git version of the core (November 17, 2015)
    
    Created by Ivan Grokhotkov, 2015.
    This example is in public domain.
*/

#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <FS.h>
#include "ESP8266httpUpdate.h"

const char* ssid = "mokiev2";
const char* password = "98765432011";

const char* host = "raw.githubusercontent.com";
const int httpsPort = 443;

// Use web browser to view and copy
// SHA1 fingerprint of the certificate
const char* fingerprint = "70 0B 6F 62 4F 41 EB 1A 42 3F 73 5A DA 96 98 2D 7F 2B 75 6F";

const char* url = "/dimiork/esp-ota/master/WiFiScan.ino.nodemcu.bin";

void setup() {
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  WiFi.mode(WIFI_STA);
  delay(5000);
  Serial.println();
  Serial.print("connecting to ");
  Serial.println(ssid);

//  if (WiFi.SSID() != ssid) {
//    WiFi.begin(ssid, password);
//  }
  while (WiFi.status() != WL_CONNECTED) {
    WiFi.begin(ssid, password);
    Serial.println(".");
    // wait 10 seconds for connection:
    delay(10000);
//    delay(500);
    
    Serial.println(WiFi.status());
    Serial.println(WL_CONNECTED);
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  // configure time
  configTime(3 * 3600, 0, "pool.ntp.org");

  // Use WiFiClientSecure class to create TLS connection
  WiFiClientSecure client;
  Serial.print("connecting to ");
  Serial.println(host);
  if (!client.connect(host, httpsPort)) {
    Serial.println("connection failed");
    return;
  }

  if (client.verify(fingerprint, host)) {
    Serial.println("certificate matches");
  } else {
    Serial.println("certificate doesn't match");
    return;
  }

  Serial.print("Starting OTA from: ");
  Serial.println(url);
  
  auto ret = ESPhttpUpdate.update(client, host, url);
  // if successful, ESP will restart
  Serial.println("update failed");
  Serial.println((int) ret);
}

void loop() {
}
