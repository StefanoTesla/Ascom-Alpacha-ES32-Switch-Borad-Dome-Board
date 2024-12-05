#ifndef SWITCH_CONFIG
#define SWITCH_CONFIG


void saveSwitchConfig(){
    File file = LittleFS.open("/cfg/switchconfig.txt", FILE_WRITE);


    JsonDocument doc;
    JsonArray array = doc["Switches"].to<JsonArray>();

    for (size_t i = 0; i < _MAX_SWITCH_ID_ ; i++)
    {
        if(Switch.data[i].property.type == SwTypeNull  ){
            continue;
        }
        JsonObject jsonSwitch = array.add<JsonObject>();
        jsonSwitch["name"] = Switch.data[i].Name;
        jsonSwitch["description"] = Switch.data[i].Description;
        jsonSwitch["type"] = Switch.data[i].property.type;

    }

    serializeJson(doc, file);
    file.close();

}


void initSwitchConfig(){

    if (!LittleFS.exists("/cfg/switchconfig.txt")) {
        Serial.println("[ERR] Switch: unable to find switchconfig file, I'm creating a new one..");

        File file = LittleFS.open("/cfg/switchconfig.txt", FILE_WRITE);
        file.close();
        Switch.config.load.isValid = false;
        return;
    }

    saveSwitchConfig();
    return;
    File file = LittleFS.open("/cfg/switchconfig.txt", FILE_READ);
    JsonDocument doc;

    DeserializationError error = deserializeJson(doc, file);
    
    if(error){
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.c_str());
        CoverC.config.load.isValid = false;
        return;
    }
    file.close();

    

}




#endif