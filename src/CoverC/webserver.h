#ifndef CC_WEBSERVER
#define CC_WEBSERVER



void coverWebServer(){

    server.on("/api/coverc/cfg", HTTP_GET, [](AsyncWebServerRequest * request) {
        AsyncJsonResponse* response = new AsyncJsonResponse();
        JsonObject doc = response->getRoot().to<JsonObject>();

        JsonObject calibrator = doc["calibrator"].to<JsonObject>();
        calibrator["present"] = CoverC.config.calibrator.present;
        calibrator["out"] = CoverC.config.calibrator.outPWM;


        JsonObject cover = doc["cover"].to<JsonObject>();
        cover["present"] = CoverC.config.cover.present;
        cover["out"] = CoverC.config.cover.outServoPin;
        cover["closeDeg"] = CoverC.config.cover.closeDeg;
        cover["openDeg"] = CoverC.config.cover.openDeg;
        cover["maxDeg"] = CoverC.config.cover.maxDeg;

        response->setLength();
        request->send(response);
    });
}

#endif