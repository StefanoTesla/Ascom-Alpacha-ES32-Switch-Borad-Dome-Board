#define ALPACA_PORT 4567

#define DOME
//#define GATE_BOARD

#define SWITCH
#define COVER_CALIBRATOR

#include <Arduino.h>
#include <stdint.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <ElegantOTA.h>
#include <ESPAsyncWiFiManager.h>  
#include "AsyncUDP.h"
#include <AsyncJson.h>
#include <ArduinoJson.h>
#include "LittleFS.h"
#include <ESP32Servo.h>

AsyncWebServer server(80);
AsyncWebServer alpaca(ALPACA_PORT);

#include "header.h"

#ifdef DOME
#include "Dome/header.h"
#endif
#ifdef COVER_CALIBRATOR
#include "CoverC/header.h"
#endif
#ifdef SWITCH
#include "Switch/header.h"
#endif




#include "Alpaca/middleware.h"
#include "Alpaca/common.h"
#include "Alpaca/apiManage.h"

#ifdef DOME
#include "Dome/main.h"
#endif
#ifdef COVER_CALIBRATOR
#include "CoverC/main.h"
#endif



DNSServer dns;
AsyncUDP udp;

#include "Alpaca/discovery.h"

void setup() {
  Serial.begin(115200);

  if(!LittleFS.begin()){
    Serial.println("An Error has occurred while mounting LittleFS");
    return;
  }
  
  AsyncWiFiManager wifiManager(&server,&dns);
  wifiManager.autoConnect("TeslaBoard");


  //start alpaca discovery
  alpacaDiscovery(udp);

  server.on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
      Serial.println(request->getAttribute("primo"));
      Serial.println(request->getAttribute("secondo"));
      request->send(200, "application/json", "{\"hello\": \"world\"}");
  }).addMiddleware(&getAlpacaID);

  AlpacaManager();
  #ifdef DOME
    domeRequestHandler();
  #endif
  #ifdef COVER_CALIBRATOR
  coverCalibratorRequestHandler();
  #endif
  
  server.begin();
  alpaca.begin();
}

void loop() {
  Global.actualMillis = millis();
  // put your main code here, to run repeatedly:
  domehandlerloop();
  coverCalibratorLoop();
}
