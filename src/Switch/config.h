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

        switch(Switch.data[i].property.type){

            case SwTypePWM:
            case SwTypeServo:
            case SwTypeAInput:
            case SwTypeAOutput:
                jsonSwitch["min"] = Switch.data[i].property.minValue;
                jsonSwitch["max"] = Switch.data[i].property.maxValue;

            case SwTypeDInput:
            case SwTypeDOutput:
                jsonSwitch["pin"] = Switch.data[i].property.pin;
                break;
            default:
                break;
        }
    }

    serializeJson(doc, file);
    file.close();

}


void initSwitchConfig(){
    Serial.println("INIT: switch config reading..");
    if (!LittleFS.exists("/cfg/switchconfig.txt")) {
        Serial.println("[ERR] Switch: unable to find switchconfig file, I'm creating a new one..");

        File file = LittleFS.open("/cfg/switchconfig.txt", FILE_WRITE);
        file.close();
        Switch.config.load.isValid = false;
        return;
    }

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