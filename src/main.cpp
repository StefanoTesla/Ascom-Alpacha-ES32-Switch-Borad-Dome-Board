#define ALPACA_PORT 4567

#define DOME
#ifdef DOME
	#define GATE_BOARD
#endif

#define SWITCH
#define COVER_CALIBRATOR

#define BROWSER_LANG "it"

#include "libraries.h"

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
#ifdef SWITCH
#include "Switch/main.h"
#endif

#include "Board/configuration.h"
#include "Board/webserver.h"

DNSServer dns;
AsyncUDP udp;

#include "Alpaca/discovery.h"

void setup() {
  Serial.begin(115200);
  
  if(!LittleFS.begin()){
    Serial.println("An Error has occurred while mounting LittleFS");
    return;
  }

  startupTask();
  
  AsyncWiFiManager wifiManager(&server,&dns);
  wifiManager.autoConnect("TeslaBoard");


  //start alpaca discovery
  alpacaDiscovery(udp);

  server.serveStatic("/index", LittleFS, "/index.html");
  
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

  boardWebServer();
  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");
  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Methods", "GET, POST, PUT");
  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Headers", "Content-Type");
  
  server.onNotFound([](AsyncWebServerRequest *request) {
    if (request->method() == HTTP_OPTIONS) {
      request->send(200);
    } else {
      request->send(404);
    }
  });
  
  server.begin();
  alpaca.begin();
}

void loop() {
  Global.actualMillis = millis();
  // put your main code here, to run repeatedly:
  domeLoop();
  coverCalibratorLoop();
  SwitchLoop();
}
