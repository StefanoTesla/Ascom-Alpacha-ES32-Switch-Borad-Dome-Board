#ifndef CC_CONFIG
#define CC_CONFIG

void initCoverCConfig(){
    JsonDocument doc;
    File file = LittleFS.open("/cfg/ccconfig.txt", FILE_WRITE);
    if (!file) {
        Serial.println("Reading Cover Calibrator config error");
        return;
    }
    DeserializationError error = deserializeJson(doc, file);
    file.close();
    if(error){
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.c_str());
        return;
    }

    JsonObject calibrator = doc["Calibrator"];
    CoverC.config.calibrator.present = calibrator["present"];
    CoverC.config.calibrator.outPWM = calibrator["pin"];

    JsonObject cover = doc["Cover"];
    CoverC.config.cover.present = cover["present"];
    CoverC.config.cover.outServoPin = cover["pin"];
    CoverC.config.cover.movingTime = cover["movingTime"];
    CoverC.config.cover.openDeg = cover["openDeg"];
    CoverC.config.cover.closeDeg = cover["closeDeg"];
    CoverC.config.cover.maxDeg = cover["maxDeg"];
    
    if(CoverC.config.calibrator.present){ 
        ledcSetup(0, 20000, 10);
        ledcAttachPin(CoverC.config.calibrator.outPWM, 0);
    }

    if(CoverC.config.cover.present){
        ledcSetup(8, 50, 8);
        ledcAttachPin(CoverC.config.calibrator.outPWM, 8);  
    }

    CoverC.config.load.isValid = true;
}

void saveCoverCConfig(){

    File file = LittleFS.open("/cfg/ccconfig.txt", FILE_WRITE);

    if (!file) {
        Serial.println("Error during open CoverCalibration config file");
        return;
    }
    JsonDocument doc;
    JsonObject cover = doc["Cover"].to<JsonObject>();
    cover["present"] = CoverC.config.cover.present;
    cover["pin"] = CoverC.config.cover.outServoPin;
    cover["movingTime"] = CoverC.config.cover.movingTime;
    cover["openDeg"] = CoverC.config.cover.openDeg;
    cover["closeDeg"] = CoverC.config.cover.closeDeg;
    cover["maxDeg"] = CoverC.config.cover.maxDeg;

    JsonObject calibrator = doc["Calibrator"].to<JsonObject>();
    cover["present"] = CoverC.config.calibrator.present;
    cover["pin"] = CoverC.config.calibrator.outPWM;

    serializeJson(doc, file);
    file.close();

}


#endif