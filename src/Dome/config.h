#ifndef DOME_CONFIG
#define DOME_CONFIG


void saveDomeConfig(){

    File file = LittleFS.open("/cfg/domeconfig.txt", FILE_WRITE);
    if (!file) {
        Serial.println("Error during open Dome config file");
        return;
    }
    JsonDocument doc;

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
    doc["movTimeOut"] = Dome.config.data.movingTimeOut;

    serializeJson(doc, file);

    Dome.config.Save.execute = false;
    file.close();
    
}


void initDomeConfig(){
    Serial.println("INIT: Reading Dome config...");
    JsonDocument doc;
    File file = LittleFS.open("/cfg/domeconfig.txt", FILE_READ);

    if (!file) {
        Serial.println("[ERR] Init: Reading Dome config error");
        saveDomeConfig();
        return;
    }

    DeserializationError error = deserializeJson(doc, file);
    if(error){
        Serial.print(F("[ERR] Init: Reading Dome config deserializeJson() failed: "));
        Serial.println(error.c_str());
        file.close();
        return;
    }
    file.close();

    JsonObject pinOpen = doc["pinOpen"];
    Dome.config.data.inOpen.pin = pinOpen["pin"]; 
    Dome.config.data.inOpen.delayON = pinOpen["dOn"]; 
    Dome.config.data.inOpen.delayOFF = pinOpen["dOff"];
    Dome.config.data.inOpen.type = pinOpen["type"];

    JsonObject pinClose = doc["pinClose"];
    Dome.config.data.inClose.pin = pinClose["pin"]; 
    Dome.config.data.inClose.delayON = pinClose["dOn"]; 
    Dome.config.data.inClose.delayOFF = pinClose["dOff"];
    Dome.config.data.inClose.type = pinClose["type"];

    JsonObject autoClose = doc["autoclose"];
    Dome.config.data.enAutoClose = autoClose["enable"];
    Dome.config.data.autoCloseTimeOut = autoClose["minutes"];

    Dome.config.data.outStart_Open = doc["pinStart"];
    Dome.config.data.outHalt_Close = doc["pinHalt"];
    Dome.config.data.movingTimeOut = doc["movTimeOut"];

    Dome.config.Load.isValid = true;

    pinMode(Dome.config.data.outStart_Open, OUTPUT);
    pinMode(Dome.config.data.outHalt_Close, OUTPUT);
    pinMode(Dome.config.data.inOpen.pin, INPUT);
    pinMode(Dome.config.data.inClose.pin, INPUT);

}


#endif