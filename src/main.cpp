#define ALPACA_PORT 4567

#define DOME
#ifdef DOME
	#define GATE_BOARD
#endif
#define COVER_CALIBRATOR
#define SWITCH

#include "libraries.h"

AsyncWebSocket ws("/ws");
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

#include "Board/webserver.h"
#include "Board/main.h"

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
  server.addHandler(&ws);
  //start alpaca discovery
  alpacaDiscovery(udp);
  AlpacaManager();
  #ifdef DOME
    domeRequestHandler();
  #endif
  #ifdef COVER_CALIBRATOR
    coverCalibratorRequestHandler();
  #endif
  #ifdef SWITCH
    switchRequestHandler();
  #endif
  
  boardWebServer();

  server.serveStatic("/", LittleFS, "/www/").setDefaultFile("index.html");
  server.serveStatic("/assets/", LittleFS, "/www/assets/").setCacheControl("max-age=604800");
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
  
  boardLoop();
  #ifdef DOME
  domeLoop();
  #endif
  #ifdef COVER_CALIBRATOR
  coverCalibratorLoop();
  #endif
  #ifdef SWITCH
  SwitchLoop();
  #endif
}
