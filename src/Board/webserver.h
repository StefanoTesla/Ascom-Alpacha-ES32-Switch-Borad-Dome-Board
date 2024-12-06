#ifndef BOARD_WEBSERVER
#define BOARD_WEBSERVER


void boardWebServer(){

    server.on("/api/cfg", HTTP_GET, [](AsyncWebServerRequest * request) {
        AsyncJsonResponse* response = new AsyncJsonResponse();
        JsonObject doc = response->getRoot().to<JsonObject>();

        JsonObject board = doc["board"].to<JsonObject>();
        board["locale"] = Global.config.language.locale;

        JsonObject wifi = board["wifi"].to<JsonObject>();
        wifi["reconTime"] = 2000;
        wifi["enStaticIP"] = false;
        JsonArray ip = wifi["ip"].to<JsonArray>();
        ip[0] = 192;
        ip[1] = 168;
        ip[2] = 0;
        ip[3] = 192;
        JsonArray sub = wifi["sub"].to<JsonArray>();
        sub[0] = 192;
        sub[1] = 168;
        sub[2] = 0;
        sub[3] = 192;

        JsonObject define = doc["define"].to<JsonObject>();
        #ifdef DOME
        define["dome"] = true;
        #else
        define["dome"] = false;
        #endif
        #ifdef SWITCH
        define["switch"] = true;
        #else
        define["switch"] = false;
        #endif
        #ifdef COVER_CALIBRATOR
        define["coverc"] = true;
        #else
        define["coverc"] = false;
        #endif

        response->setLength();
        request->send(response);
    });


    server.on("/api/board/status", HTTP_GET, [](AsyncWebServerRequest * request) {
        AsyncJsonResponse* response = new AsyncJsonResponse();
        JsonObject doc = response->getRoot().to<JsonObject>();

        JsonObject wifi = doc["wifi"].to<JsonObject>();
        wifi["uptime"] = Global.config.wifi.upTime.minutes;
        wifi["ssid"] = WiFi.SSID();
        wifi["db"] = WiFi.RSSI();
        wifi["ip"] = WiFi.localIP();

        JsonObject memo = doc["memory"].to<JsonObject>();
        memo["heapSize"] = ESP.getHeapSize();
        memo["freeHeap"] = ESP.getFreeHeap();
        memo["minHeap"] = ESP.getMinFreeHeap();

        JsonObject board = doc["board"].to<JsonObject>();
        board["uptime"] = Global.config.esp32.upTime.minutes;
        board["cpu"] = ESP.getChipModel();
        board["cores"] = ESP.getChipCores();
        board["rev"] = ESP.getChipRevision();
        board["freq"] = ESP.getCpuFreqMHz();

        response->setLength();
        request->send(response);
    });


    AsyncCallbackJsonWebHandler* boardConfigHandler = new AsyncCallbackJsonWebHandler("/api/board/cfg");

    boardConfigHandler->setMethod(HTTP_POST | HTTP_PUT);
    boardConfigHandler->onRequest([](AsyncWebServerRequest* request, JsonVariant& json) {
            AsyncJsonResponse* response = new AsyncJsonResponse();
            JsonObject doc = response->getRoot().to<JsonObject>();
            JsonArray err = doc["errors"].to<JsonArray>();
            bool error = false;
            bool reboot = false;
            
            Global.config.language.locale = json["locale"].as<String>();
            Global.config.save.execute = true;
            response->setLength();
            request->send(response);
        });

    server.addHandler(boardConfigHandler);

    server.serveStatic("/board/boarcfg.txt", LittleFS, "/cfg/boarcfg.txt");
}

#endif