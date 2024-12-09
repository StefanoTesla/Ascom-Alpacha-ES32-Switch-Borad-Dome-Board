#ifndef SWITCH_WEBSERVER
#define SWITCH_WEBSERVER


void switchWebServer(){

    server.on("/api/switch/cfg", HTTP_GET, [](AsyncWebServerRequest * request) {
        AsyncJsonResponse* response = new AsyncJsonResponse();
        JsonObject doc = response->getRoot().to<JsonObject>();

        JsonArray array = doc["Switches"].to<JsonArray>();

        for(int i=0;i<_MAX_SWITCH_ID_;i++){
            JsonObject jsonSwitch = array.add<JsonObject>();
            jsonSwitch["name"] = Switch.data[i].Name;
            jsonSwitch["description"] = Switch.data[i].Description;
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
            bool error = false;
            bool reboot = false;
            
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