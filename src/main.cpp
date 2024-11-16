#define DOME

#ifdef DOME
/*
Usually I expect to have a gate board, but in the last time I found some guys that handle the motor with two realys, open and close
Only in this case comment the #define GATE_BOARD
#start pin will be open command
#halt pin will be close command
*/
#define GATE_BOARD
#endif

#define SWITCH
#define COVERC

#include <WiFi.h>
#include "AsyncJson.h"
#include "AsyncUDP.h"
#include "HTTPClient.h"
#include <ESPAsyncWebServer.h>
#include <ESPAsyncWiFiManager.h>
#include <stdint.h>
#include "SPIFFS.h"
#include "Dome/domeVariable.h"
#include "Switches/switchVariable.h"
#include "CoverC/coverVariable.h"
#include "header.h"
#include <ElegantOTA.h>

AsyncWebServer server(80);
AsyncWebServer Alpserver(4567);

#include "Alpaca/AlpacaManageFunction.h"
#ifdef DOME
#include "Dome/dome.h"
#endif

#ifdef SWITCH
#include "Switches/switch.h"
#endif

#ifdef COVERC
#include "CoverC/cover.h"
#endif
#include "browserServer.h"
#include "configuration.h"

#include "loop.h"
DNSServer dns;
AsyncUDP udp;

void ServerNotFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
  Serial.println("404");
  Serial.println(request->url());
}

void setup()
{
  Serial.begin(115200);
  AlpacaData.serverTransactionID = 0;
/* reading configuration from file */
  if (!SPIFFS.begin()) { Serial.println("An Error has occurred while mounting SPIFFS"); return; }

  #ifdef DOME
  initDomeConfig();
  Serial.println("dome init done");
  #endif

  #ifdef SWITCH
  initSwitchConfig();
  Serial.println("switch init done");
  #endif

  #ifdef COVERC
  initCoverCConfig();
  Serial.println("cover init done");
  #endif

  Serial.println("Listening for discovery requests...");
  AsyncWiFiManager wifiManager(&server,&dns);
  wifiManager.autoConnect();
  Serial.print("Connect with IP Address: ");
  Serial.println(WiFi.localIP());


  if (udp.listen(32227))
  {
    Serial.println("Listening for discovery requests...");
    udp.onPacket([](AsyncUDPPacket packet) {
      if (packet.length() < 16)
      {
        return;
      }
      //Compare packet to Alpaca Discovery string
      if (strncmp("alpacadiscovery1", (char *)packet.data(), 16) != 0)
      {
        return;
      }
      packet.printf("{\"alpacaport\": 4567}");
    });
  }

  Alpserver.onNotFound(notFound);
  /*** MANAGE AREA ***/

  AlpacaManager();
  #ifdef DOME
  domeServer();
  #endif
  
  #ifdef SWITCH
  switchServer();
  #endif

  #ifdef COVERC
  coverServer();
  #endif

  browserServer();

  /** END SWITCH SPECIFIC METHODS **/
  Alpserver.begin();
  ElegantOTA.begin(&server);
  server.begin();
  Dome.lastCommunicationMillis = millis();
  Serial.println("setup done");
}

void loop(){
  main_loop();
}
