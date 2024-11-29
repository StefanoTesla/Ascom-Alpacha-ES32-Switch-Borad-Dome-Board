#ifndef BOARD_WEBSERVER
#define BOARD_WEBSERVER


void boardWebServer(){


    server.on("/api/cfg", HTTP_GET, [](AsyncWebServerRequest * request) {
        AsyncJsonResponse* response = new AsyncJsonResponse();
        JsonObject doc = response->getRoot().to<JsonObject>();

        doc["locale"] = BROWSER_LANG;
        JsonObject define = doc["define"].to<JsonObject>();
        #ifdef DOME
        define["dome"] = true;
        #endif
        #ifdef SWITCH
        define["switch"] = true;
        #endif
        #ifdef COVER_CALIBRATOR
        define["coverc"] = true;
        #endif

        response->setLength();
        request->send(response);
    });


}

#endif