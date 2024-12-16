#ifndef SWITCH_WEBSERVER
#define SWITCH_WEBSERVER
#include "libraries.h"
#include "header.h"

void switchWebServer(){

    server.on("/api/switch/cfg", HTTP_GET, [](AsyncWebServerRequest * request) {
        AsyncJsonResponse* response = new AsyncJsonResponse();
        JsonObject doc = response->getRoot().to<JsonObject>();

        JsonArray array = doc["Switches"].to<JsonArray>();

        for(int i=0;i<_MAX_SWITCH_ID_;i++){
            if(Switch.data[i].property.type == SwTypeNull  ){
                continue;
            }
            JsonObject jsonSwitch = array.add<JsonObject>();
            jsonSwitch["name"] = Switch.data[i].property.Name;
            jsonSwitch["desc"] = Switch.data[i].property.Description;
            jsonSwitch["type"] = Switch.data[i].property.type;
            jsonSwitch["pin"] = Switch.data[i].property.pin;
            jsonSwitch["min"] = Switch.data[i].property.minValue;
            jsonSwitch["max"] = Switch.data[i].property.maxValue;

        }
        
        response->setLength();
        request->send(response);
    });

    server.on("/api/switch/status", HTTP_GET, [](AsyncWebServerRequest * request) {
        AsyncJsonResponse* response = new AsyncJsonResponse();
        JsonObject doc = response->getRoot().to<JsonObject>();

        JsonArray array = doc["Switches"].to<JsonArray>();

        for(int i=0;i<_MAX_SWITCH_ID_;i++){
            if(Switch.data[i].property.type == SwTypeNull  ){
                continue;
            }
            JsonObject jsonSwitch = array.add<JsonObject>();
            jsonSwitch["name"] = Switch.data[i].property.Name;
            jsonSwitch["desc"] = Switch.data[i].property.Description;
            jsonSwitch["type"] = Switch.data[i].property.type;
            jsonSwitch["min"] = Switch.data[i].property.minValue;
            jsonSwitch["max"] = Switch.data[i].property.maxValue;
            if( Switch.data[i].actualValue.boValue ){
                jsonSwitch["boValue"] = true;   
            } else {
                jsonSwitch["boValue"] = false;   
            }
            
            jsonSwitch["intValue"] = Switch.data[i].actualValue.intValue;
        }
        
        response->setLength();


        request->send(response);
    });

    server.on("/api/switch/set-value", HTTP_POST, [](AsyncWebServerRequest * request) {
        AsyncJsonResponse* response = new AsyncJsonResponse();
        JsonObject doc = response->getRoot().to<JsonObject>();
        String parameter;
        doc["execute"] = false;
        bool err = false;
        bool exist = false;
        bool inRange = false;
        int id = -1;
        int value = 0;
        int paramsNr = request->params();
        
        for (int i = 0; i < paramsNr; i++) {
            const AsyncWebParameter* p = request->getParam(i);
            parameter = p->name();
            if (parameter == "id") {
                id = p->value().toInt();
            } 
            if (parameter == "value") {
                exist = true;
                value = p->value().toInt();
            } 
        }
        if(id < 0 || id >= _MAX_SWITCH_ID_){
            doc["error"] = "ID out of range";
            err= true;
        }
        if(exist){
            if(value < Switch.data[id].property.minValue){
                doc["error"] = "value excede the minimum";
                err= true;
            }
            if(value > Switch.data[id].property.maxValue){
                doc["error"] = "value excede the maximum";
                err= true;
            }
        } else {
            doc["error"] = "value not provided";
            err= true;
        }


        if(!err){
            Switch.data[id].command.intValue = value;
            if(value == Switch.data[id].property.minValue){
                Switch.data[id].command.boValue = false;
            } else {
                Switch.data[id].command.boValue = true;
            }
            Switch.data[id].command.execute = true;
            doc["execute"] = true;
        }

        response->setLength();
        request->send(response);
    });

    AsyncCallbackJsonWebHandler* switchConfigHandler = new AsyncCallbackJsonWebHandler("/api/switch/cfg");

    switchConfigHandler->setMethod(HTTP_POST | HTTP_PUT);
    switchConfigHandler->onRequest([](AsyncWebServerRequest* request, JsonVariant& json) {
            AsyncJsonResponse* response = new AsyncJsonResponse();
            JsonObject doc = response->getRoot().to<JsonObject>();
            JsonArray err = doc["errors"].to<JsonArray>();

            bool error = false;
            bool reboot = false;
            int count = 0;
            unsigned int type = 0;

            for (JsonObject Switche : json["Switches"].as<JsonArray>()) {
                SwitchProperty propertyStruct;

                if(!Switche["name"].is<String>() || !Switche["type"].is<unsigned int>() || !Switche["pin"].is<unsigned int>()){
                    err.add("Name or type or pin not defined");
                    continue;
                }

                type = Switche["type"].as<unsigned int>();

                if(type == 0 and type > 6){
                    err.add("Switch of type 0 skipped");
                    continue;
                }
                propertyStruct.type = static_cast<SwitchType>(type);

                if (propertyStruct.type == SwTypeDInput){
                    Serial.println("New Input");
                    propertyStruct.pin = Switche["pin"].as<unsigned int>();
                    if(!pinUsableAsInput(propertyStruct.pin)){
                        err.add("pin not usable as input");
                        continue;
                    }
                } else if ( propertyStruct.type == SwTypeDOutput ) {
                    Serial.println("New Output");
                    propertyStruct.pin = Switche["pin"].as<unsigned int>();
                    if(!pinUsableAsOutput(propertyStruct.pin)){
                        err.add("pin not usable as output");
                        continue;
                    }
                } else if ( propertyStruct.type == SwTypePWM ) {
                    Serial.println("New PWM");
                    propertyStruct.pin = Switche["pin"].as<unsigned int>();
                    if(!pinUsableAsOutput(propertyStruct.pin)){
                        err.add("pin not usable as pwm output");
                        continue;
                    }    
                    if(!Switche["min"].is<unsigned int>() || !Switche["max"].is<unsigned int>()){
                        err.add("Min or Max value not defined");
                        continue;
                    }
                    propertyStruct.minValue = Switche["min"];
                    propertyStruct.maxValue = Switche["max"];

                } else if ( propertyStruct.type == SwTypeServo ) {
                    Serial.println("New Servo");
                    propertyStruct.pin = Switche["pin"].as<unsigned int>();
                    if(!pinUsableAsOutput(propertyStruct.pin)){
                        err.add("pin not usable as servo output");
                        continue;
                    }    
                    if(!Switche["min"].is<unsigned int>() || !Switche["max"].is<unsigned int>()){
                        err.add("Min or Max value not defined");
                        continue;
                    }
                    propertyStruct.minValue = Switche["min"];
                    propertyStruct.maxValue = Switche["max"];

                } else {
                    Serial.println("not implemented");
                }

                const char* jsonName = Switche["name"].as<const char*>();
                strncpy(propertyStruct.Name, jsonName, sizeof(propertyStruct.Name) - 1);
                propertyStruct.Name[sizeof(propertyStruct.Name) - 1] = '\0';

                const char* jsonDesc = Switche["desc"].as<const char*>();
                strncpy(propertyStruct.Description, jsonDesc, sizeof(propertyStruct.Description) - 1);
                propertyStruct.Description[sizeof(propertyStruct.Description) - 1] = '\0';
                
                Serial.println(count);
                Serial.print("Nome: ");
                Serial.println(propertyStruct.Name);
                Serial.print("Descrizione: ");
                Serial.println(propertyStruct.Description);
                Serial.print("Tipo: ");
                Serial.println(propertyStruct.type);
                Serial.print("Pin: ");
                Serial.println(propertyStruct.pin);
                Switch.config.tmp[count].property = propertyStruct;
                count +=1;
            }


            if(!error){
                Switch.config.save.execute = true;
            } else {
                response->setCode(500);
            }
            doc["reboot"] = reboot;

            response->setLength();
            request->send(response);
        });

    server.addHandler(switchConfigHandler);

    server.serveStatic("/switch/switchconfig.txt", LittleFS, "/cfg/switchconfig.txt");
}

#endif