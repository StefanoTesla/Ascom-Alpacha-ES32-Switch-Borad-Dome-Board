#ifndef BOARD_CONFIG
#define BOARD_CONFIG

void saveBoardConfig(){

    JsonDocument doc;

    doc["locale"] = Global.config.language.locale;
    
    JsonObject wifi = doc["wifi"].to<JsonObject>();
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

    File file = LittleFS.open("/cfg/boarcfg.txt",FILE_WRITE);

    if (!file) {
        Serial.println("Error during open board config file");
        return;
    }

    serializeJson(doc,file);

    file.close();
    Global.config.save.execute = false;
    
}


void initBoardConfig(){

    File file = LittleFS.open("/cfg/boarcfg.txt",FILE_READ);

    if (!file) {
        Serial.println("Error during open board config file");
        return;
    }

    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, file);

    if(error){
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.c_str());
        return;
    }

    Global.config.language.locale = doc["locale"].as<String>();  
    Serial.print("Language:");
    Serial.println(Global.config.language.locale);
}






#endif