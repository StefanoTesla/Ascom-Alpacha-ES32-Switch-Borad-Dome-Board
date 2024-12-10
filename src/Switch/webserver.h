#ifndef SWITCH_WEBSERVER
#define SWITCH_WEBSERVER
#include "libraries.h"
#include "header.h"

void switchWebServer(){

    server.on("/api/switch/cfg", HTTP_GET, [](AsyncWebServerRequest * request) {
        AsyncJsonResponse* response = new AsyncJsonResponse();
        JsonObject doc = response->getRoot().to<JsonObject>();

        JsonArray array = doc["Switches"].to<JsonArray>();

        for(int i=0;i<_MAX_SWITCH_ID_;i++){
            JsonObject jsonSwitch = array.add<JsonObject>();
            jsonSwitch["name"] = Switch.data[i].Name;
            jsonSwitch["desc"] = Switch.data[i].Description;
            jsonSwitch["type"] = Switch.data[i].property.type;
            if(Switch.data[i].property.type == SwTypeNull  ){
                continue;
            }
        }
        
        response->setLength();
        request->send(response);
    });

    server.on("/api/coverc/status", HTTP_GET, [](AsyncWebServerRequest * request) {
        AsyncJsonResponse* response = new AsyncJsonResponse();
        JsonObject doc = response->getRoot().to<JsonObject>();

        request->send(response);
    });





    AsyncCallbackJsonWebHandler* switchConfigHandler = new AsyncCallbackJsonWebHandler("/api/switch/cfg");

    switchConfigHandler->setMethod(HTTP_POST | HTTP_PUT);
    switchConfigHandler->onRequest([](AsyncWebServerRequest* request, JsonVariant& json) {
            AsyncJsonResponse* response = new AsyncJsonResponse();
            JsonObject doc = response->getRoot().to<JsonObject>();
            JsonArray err = doc["errors"].to<JsonArray>();
            SwitchArrayStruct tmpStruct[1];

            bool error = false;
            bool reboot = false;
            int count = 0;
            unsigned int type = 0;
            serializeJson(json,Serial);

            for (JsonObject Switche : json["Switches"].as<JsonArray>()) {
                serializeJson(Switche,Serial);
                type = Switche["type"].as<unsigned int>();
                Serial.println(type);
                if(type !=0 and type <= 6){
                    Serial.println("good");
                    tmpStruct[0].Name = Switche["name"].as<String>();
                    tmpStruct[0].Description = Switche["desc"].as<String>(); 
                    tmpStruct[0].property.type = static_cast<SwitchType>(type);
                    tmpStruct[0].property.pin = Switche["pin"].as<unsigned int>();
                    count +=1;
                } else {
                    Serial.println("saltato, tipo 0");
                }
            Serial.println(count);
            }


            if(!error){

            } else {
                response->setCode(500);
            }
            doc["reboot"] = reboot;

            response->setLength();
            request->send(response);
        });

    server.addHandler(switchConfigHandler);

    server.serveStatic("/switch/switchconfig.txt", LittleFS, "/cfg/switchcfg.txt");
}

#endif