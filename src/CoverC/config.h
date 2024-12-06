#ifndef CC_CONFIG
#define CC_CONFIG

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
    calibrator["present"] = CoverC.config.calibrator.present;
    calibrator["pin"] = CoverC.config.calibrator.outPWM;

    serializeJson(doc, file);
    file.close();

}

void initCoverCConfig(){
    Serial.println("INIT: Reading cover calibrator config...");
    JsonDocument doc;
    File file = LittleFS.open("/cfg/ccconfig.txt", FILE_READ);
    if (!file) {
        Serial.println("[ERR] INIT: Reading Cover Calibrator config error");
        return;
    }
    DeserializationError error = deserializeJson(doc, file);
    
    if(error){
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.c_str());
        CoverC.config.load.isValid = false;
        return;
    }
    file.close();
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
    
    unsigned int tmpCh = 666;
    if(CoverC.config.calibrator.present){ 
        tmpCh = assignLedChannel(pwm);
        if(tmpCh < 16){
            CoverC.config.calibrator.pwmChannel = tmpCh;
            ledcAttachPin(CoverC.config.calibrator.outPWM, tmpCh);
        } 
    }

    tmpCh = 666;

    if(CoverC.config.cover.present){
    tmpCh = assignLedChannel(servo);
        if(tmpCh < 16){
            CoverC.config.cover.pwmChannel = tmpCh;
            ledcAttachPin(CoverC.config.cover.outServoPin, tmpCh);
        }
    }

    CoverC.config.load.isValid = true;

}


#endif