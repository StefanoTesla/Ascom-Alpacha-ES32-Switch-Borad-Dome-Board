#ifndef CC_WEBSERVER
#define CC_WEBSERVER


void coverWebServer(){

    server.on("/api/coverc/cfg", HTTP_GET, [](AsyncWebServerRequest * request) {
        AsyncJsonResponse* response = new AsyncJsonResponse();
        JsonObject doc = response->getRoot().to<JsonObject>();

        JsonObject calibrator = doc["calibrator"].to<JsonObject>();
        calibrator["present"] = CoverC.config.calibrator.present;
        calibrator["pin"] = CoverC.config.calibrator.outPWM;


        JsonObject cover = doc["cover"].to<JsonObject>();
        cover["present"] = CoverC.config.cover.present;
        cover["pin"] = CoverC.config.cover.outServoPin;
        cover["closeDeg"] = CoverC.config.cover.closeDeg;
        cover["openDeg"] = CoverC.config.cover.openDeg;
        cover["maxDeg"] = CoverC.config.cover.maxDeg;
        cover["movTime"] = CoverC.config.cover.movingTime;

        response->setLength();
        request->send(response);
    });

    server.on("/api/coverc/status", HTTP_GET, [](AsyncWebServerRequest * request) {
        AsyncJsonResponse* response = new AsyncJsonResponse();
        JsonObject doc = response->getRoot().to<JsonObject>();

        JsonObject calibrator = doc["calibrator"].to<JsonObject>();
        calibrator["status"] = CoverC.status.calibrator.status;
        if(CoverC.config.calibrator.present){
            calibrator["brightness"] = CoverC.status.calibrator.actualBrightness;
        }

        JsonObject cover = doc["cover"].to<JsonObject>();
        cover["status"] = CoverC.status.cover.status;
        if(CoverC.config.cover.present){
            cover["angle"] = CoverC.status.cover.angle;
        }
        

        response->setLength();
        request->send(response);
    });
    server.on("/api/coverc/open", HTTP_POST, [](AsyncWebServerRequest * request) {
        AsyncJsonResponse* response = new AsyncJsonResponse();
        JsonObject doc = response->getRoot().to<JsonObject>();

        doc["execute"] = true;
        CoverC.command.cover.move = true;
        CoverC.command.cover.angle = CoverC.config.cover.openDeg;

        response->setLength();
        request->send(response);
    });

    server.on("/api/coverc/close", HTTP_POST, [](AsyncWebServerRequest * request) {
        AsyncJsonResponse* response = new AsyncJsonResponse();
        JsonObject doc = response->getRoot().to<JsonObject>();

        doc["execute"] = true;
        CoverC.command.cover.move = true;
        CoverC.command.cover.angle = CoverC.config.cover.closeDeg;

        response->setLength();
        request->send(response);
    });

    server.on("/api/coverc/brightness", HTTP_POST, [](AsyncWebServerRequest * request) {
        AsyncJsonResponse* response = new AsyncJsonResponse();
        JsonObject doc = response->getRoot().to<JsonObject>();
        String parameter;
        doc["execute"] = false;
        bool present = false;
        bool inRange = false;
        int value = 0;
        int paramsNr = request->params();
        
        for (int i = 0; i < paramsNr; i++) {
            const AsyncWebParameter* p = request->getParam(i);
            parameter = p->name();
            if (parameter == "brightness") {
                present = true;
                value = p->value().toInt();
                if(value >=0 || value <=4096){
                    CoverC.command.calibrator.change = true;
                    CoverC.command.calibrator.brightness = value;
                    inRange = true;
                    doc["execute"] = true;
                }
                else{
                    inRange = false;
                }
            }  
        }
        if(!inRange){
            doc["error"] = "brightness parameter not in range";
        }

        if(!present){
            doc["error"] = "brightness parameter not found";
        }


        response->setLength();
        request->send(response);
    });

    server.on("/api/coverc/on", HTTP_POST, [](AsyncWebServerRequest * request) {
        AsyncJsonResponse* response = new AsyncJsonResponse();
        JsonObject doc = response->getRoot().to<JsonObject>();

        CoverC.command.calibrator.change = true;
        CoverC.command.calibrator.brightness = 4096;
        doc["execute"] = true;

        response->setLength();
        request->send(response);
    });

    server.on("/api/coverc/off", HTTP_POST, [](AsyncWebServerRequest * request) {
        AsyncJsonResponse* response = new AsyncJsonResponse();
        JsonObject doc = response->getRoot().to<JsonObject>();
        CoverC.command.calibrator.change = true;
        CoverC.command.calibrator.brightness = 0;
        doc["execute"] = true;

        response->setLength();
        request->send(response);
    });


    AsyncCallbackJsonWebHandler* coverCConfigHandler = new AsyncCallbackJsonWebHandler("/api/coverc/cfg");

    coverCConfigHandler->setMethod(HTTP_POST | HTTP_PUT);
    coverCConfigHandler->onRequest([](AsyncWebServerRequest* request, JsonVariant& json) {
            AsyncJsonResponse* response = new AsyncJsonResponse();
            JsonObject doc = response->getRoot().to<JsonObject>();
            JsonArray err = doc["errors"].to<JsonArray>();
            bool error = false;
            bool reboot = false;
            
            /* calibrator */
            JsonObject calibrator = json.as<JsonObject>()["calibrator"];
            if( !calibrator["present"].is<bool>()){
                error=true;
                err.add("Calibrator present");
            }
            if( calibrator["pin"].is<unsigned int>() and commonValidateOutputPin(calibrator["pin"])){
                if (calibrator["pin"] != CoverC.config.calibrator.outPWM){
                    reboot = true;
                }
            } else {
                error=true;
                err.add("GPIO Calibrator pin");
            }

            /* cover */
            JsonObject cover = json.as<JsonObject>()["cover"];
            if( !cover["present"].is<bool>()){
                error=true;
                err.add("Cover enable");
            }
            if( cover["pin"].is<unsigned int>() and commonValidateOutputPin(calibrator["pin"])){
                if (calibrator["pin"] != CoverC.config.calibrator.outPWM){
                    reboot = true;
                }
            } else {
                error=true;
                err.add("GPIO Cover pin");
            }
            if( !cover["maxDeg"].is<unsigned int>() || cover["openDeg"] > 360){
                error=true;
                err.add("Cover MaxDeg");
            }
            if( !cover["closeDeg"].is<unsigned int>() || cover["openDeg"] > 360){
                error=true;
                err.add("Close Cover deg");
            }
            if( !cover["openDeg"].is<unsigned int>() || cover["openDeg"] > 360){
                error=true;
                err.add("Open Cover deg");
            }
            if( !cover["movTime"].is<unsigned int>()){
                error=true;
                err.add("mov time out");
            }


            if(!error){
                /* input */
                CoverC.config.calibrator.present = calibrator["present"];
                CoverC.config.calibrator.outPWM = calibrator["pin"];
                CoverC.config.cover.present = cover["present"];
                CoverC.config.cover.outServoPin = cover["pin"];
                CoverC.config.cover.maxDeg = cover["maxDeg"];
                CoverC.config.cover.closeDeg = cover["closeDeg"];
                CoverC.config.cover.openDeg = cover["openDeg"];
                CoverC.config.cover.movingTime = cover["movTime"];

                CoverC.config.save.execute = true;
            } else {
                response->setCode(500);
            }
            doc["reboot"] = reboot;

            response->setLength();
            request->send(response);
        });

    server.addHandler(coverCConfigHandler);

    server.serveStatic("/coverc/ccconfig.txt", LittleFS, "/cfg/ccconfig.txt");
}

#endif