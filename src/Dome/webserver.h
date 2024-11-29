#ifndef DOME_WEBSERVER
#define DOME_WEBSERVER


void domeWebServer(){

    server.on("/api/dome/cfg", HTTP_GET, [](AsyncWebServerRequest * request) {
        AsyncJsonResponse* response = new AsyncJsonResponse();
        JsonObject doc = response->getRoot().to<JsonObject>();

        JsonObject pinOpen = doc["pinOpen"].to<JsonObject>();
        pinOpen["pin"] = Dome.config.data.inOpen.pin;
        pinOpen["dOn"] = Dome.config.data.inOpen.delayON;
        pinOpen["dOff"] = Dome.config.data.inOpen.delayOFF;
        pinOpen["type"] = Dome.config.data.inOpen.type;

        JsonObject pinClose = doc["pinClose"].to<JsonObject>();
        pinClose["pin"] = Dome.config.data.inClose.pin;
        pinClose["dOn"] = Dome.config.data.inClose.delayON;
        pinClose["dOff"] = Dome.config.data.inClose.delayOFF;
        pinClose["type"] = Dome.config.data.inClose.type;

        JsonObject autoclose = doc["autoclose"].to<JsonObject>();
        autoclose["enable"] = Dome.config.data.enAutoClose;
        autoclose["minutes"] = Dome.config.data.autoCloseTimeOut;

        doc["pinStart"] = Dome.config.data.outStart_Open;
        doc["pinHalt"] = Dome.config.data.outHalt_Close;
        doc["movTimeOut"] = Dome.config.data.movingTimeOut / 1000;
        #ifdef GATE_BOARD
            doc["driverType"] = 0;
        #else
            doc["driverType"] = 1;
        #endif
        response->setLength();
        request->send(response);
    });


        server.on("/api/dome/status", HTTP_GET, [](AsyncWebServerRequest * request) {
        AsyncJsonResponse* response = new AsyncJsonResponse();
        JsonObject doc = response->getRoot().to<JsonObject>();

        JsonObject status = doc["status"].to<JsonObject>();
        status["roofState"] = Dome.Shutter.status;
        status["actualCommand"] = Dome.Shutter.command;
        status["lastCommand"] = Dome.Shutter.LastDomeCommand;

        JsonObject input = doc["input"].to<JsonObject>();
        input["open"] = Dome.config.data.inClose.pin;
        input["close"] = Dome.config.data.inClose.delayON;

        response->setLength();
        request->send(response);
    });



    AsyncCallbackJsonWebHandler* domeConfigHandler = new AsyncCallbackJsonWebHandler("/api/dome/cfg");

    domeConfigHandler->setMethod(HTTP_POST | HTTP_PUT);
    domeConfigHandler->onRequest([](AsyncWebServerRequest* request, JsonVariant& json) {
        AsyncJsonResponse* response = new AsyncJsonResponse();
        JsonObject doc = response->getRoot().to<JsonObject>();
        JsonArray err = doc["errors"].to<JsonArray>();
        bool error = false;
        bool reboot = false;
        
        /* open pin */
        JsonObject pinOpen = json.as<JsonObject>()["pinOpen"];
        if( pinOpen["pin"].is<unsigned int>() and commonValidateInputPin(pinOpen["pin"])){
            if (pinOpen["pin"] != Dome.config.data.inOpen.pin){
                reboot = true;
            }
        } else {
            error=true;
            err.add("GPIO Open Input");
        }

        if( !pinOpen["dOn"].is<unsigned long>() ){
            error=true;
            err.add("Open Input delay ON error");
        }

        if( !pinOpen["dOff"].is<unsigned long>()){
            error=true;
            err.add("Open Input delay OFF error");
        }

        if( !pinOpen["type"].is<bool>()){
            error=true;
            err.add("Open Input type");
        }

        /* close pin */
        JsonObject pinClose = json.as<JsonObject>()["pinClose"];

        if( pinClose["pin"].is<unsigned int>() and commonValidateInputPin(pinClose["pin"])){
            if (pinClose["pin"] != Dome.config.data.inClose.pin){
                reboot = true;
            }
        } else {
            error=true;
            err.add("GPIO Close Input");
        }
        if( !pinClose["dOn"].is<unsigned int>() ){
            error=true;
            err.add("Open Close delay ON error");
        }
        if( !pinClose["dOff"].is<unsigned int>()){
            error=true;
            err.add("Open Close delay OFF error");
        }
        if( !pinClose["type"].is<bool>()){
            error=true;
            err.add("Close pin Type error");
        }

        /* outputs */
        if( json["pinStart"].is<unsigned int>() and commonValidateInputPin(json["pinStart"])){
            if (json["pinStart"] != Dome.config.data.outStart_Open){
                reboot = true;
            }
        } else {
            error=true;
            err.add("GPIO Start Output");
        }

        if( json["pinHalt"].is<unsigned int>() and commonValidateInputPin(json["pinHalt"])){
            if (json["pinHalt"] != Dome.config.data.outHalt_Close){
                reboot = true;
            }
        } else {
            error=true;
            err.add("GPIO HALT Output");
        }

        /* timeout */
        if( !json["movTimeOut"].is<unsigned int>()){
            error=true;
            err.add("Move Time Out");
        }

        /* auto close*/
        JsonObject autoClose = json.as<JsonObject>()["autoclose"];
        if( !autoClose["enable"].is<bool>()){
            error=true;
            err.add("Enable Auto Close");
        }
        if( !autoClose["minutes"].is<unsigned int>()){
            error=true;
            err.add("Minutes for Auto Close");
        }

        if(!error){
            /* input */
            Dome.config.data.inOpen.pin = pinOpen["pin"].as<unsigned int>();
            Dome.config.data.inOpen.delayON = pinOpen["dOn"].as<unsigned long>();
            Dome.config.data.inOpen.delayOFF = pinOpen["dOff"].as<unsigned long>();
            Dome.config.data.inOpen.type = pinOpen["type"].as<bool>();
            Dome.config.data.inClose.pin = pinClose["pin"].as<unsigned int>();
            Dome.config.data.inClose.delayON = pinClose["dOn"].as<unsigned long>();
            Dome.config.data.inClose.delayOFF = pinClose["dOff"].as<unsigned long>();
            Dome.config.data.inClose.type = pinClose["type"].as<bool>();
            /* output */
            Dome.config.data.outStart_Open = json["pinStart"].as<unsigned int>();
            Dome.config.data.outHalt_Close = json["pinHalt"].as<unsigned int>();
            /* timeout */
             Dome.config.data.movingTimeOut = json["movTimeOut"].as<unsigned int>() * 1000;
            /* autoclose */
            Dome.config.data.enAutoClose = autoClose["enable"].as<bool>();
            Dome.config.data.autoCloseTimeOut = autoClose["minutes"].as<unsigned int>();
            Dome.config.Save.execute = true;
        } else {
            response->setCode(500);
        }
        doc["reboot"] = reboot;

        response->setLength();
        request->send(response);
    });

    server.addHandler(domeConfigHandler);

    server.serveStatic("/dome/domeconfig.txt", LittleFS, "/cfg/domeconfig.txt");

;
}
#endif