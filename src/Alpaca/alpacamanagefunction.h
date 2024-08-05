#ifndef MANAGE_FUNC
#define MANAGE_FUNC

const char Alp_Value[] PROGMEM= "\"Value\":";
const char Alp_CliTraId[] PROGMEM= "\"ClientTransactionID\":";
const char Alp_SerTraId[] PROGMEM = "\"ServerTransactionID\":";
const char Alp_ErrN[] PROGMEM = "\"ErrorNumber\":";
const char Alp_ErrM[] PROGMEM = "\"ErrorMessage\":";
const char Alp_NoErrors[] PROGMEM ="\"ErrorNumber\": 0,\"ErrorMessage\":\"\"";

void AlpacaHeaderSchema(AsyncResponseStream *response, AlpacaCommonData parameters){
  response->print(F("{\"ClientTransactionID\":"));
  response->print(parameters.clientTransactionID);
  response->print(F(",\"ServerTransactionID\":"));
  response->print(parameters.serverTransactionID);
  response->print(F(","));
}

void AlpacaNoErrorSchema(AsyncResponseStream *response, bool comma = true){
  response->printf("%s0,%s\"\"",Alp_ErrN,Alp_ErrM);
  if(comma){response->print(F(","));}
}

void GetAlpArguments(AsyncWebServerRequest *request ) {
  AlpacaData.switches.idExist = false;
  AlpacaData.switches.intValueExist = false;
  AlpacaData.switches.nameExist = false;
  AlpacaData.switches.stateExist = false;
  AlpacaData.switches.intValueExist = false;
  AlpacaData.switches.nameExist = false;
  AlpacaData.switches.stateExist = false;
  AlpacaData.clientID = 0;
  AlpacaData.clientTransactionID = 0;
  AlpacaData.boConnect = false;
  AlpacaData.switches.id = -1;
  AlpacaData.switches.state = false;
  AlpacaData.switches.intValue = -1;
  AlpacaData.coverC.brightness = -1;
  int paramsNr = request->params();
  String parameter;
  AlpacaData.serverTransactionID++;
  for (int i = 0; i < paramsNr; i++) {
    const AsyncWebParameter* p = request->getParam(i);

    parameter = p->name();
    parameter.toLowerCase();
    if (parameter == "clientid") {
      AlpacaData.clientID = p->value().toInt();
    }
    if (parameter == "clienttransactionid") {
      AlpacaData.clientTransactionID = p->value().toInt();
    }
    if (parameter == "connected") {
      String booleanString;
      booleanString = p->value();
      booleanString.toLowerCase();      
      if ( booleanString == "true"){
        AlpacaData.boConnect = true;
      } else {
        AlpacaData.boConnect = false;
      }
    }
    if (parameter == "id") {
      AlpacaData.switches.idExist = true;
      AlpacaData.switches.id = p->value().toInt();
    }
    if (parameter == "value") {
      AlpacaData.switches.intValueExist = true;
      AlpacaData.switches.intValue = p->value().toInt();
    }
    if (parameter == "name") {
      AlpacaData.switches.nameExist = true;
      AlpacaData.switches.name = p->value();
    }
    if (parameter == "state") {
      AlpacaData.switches.stateExist = true;
      String booleanString;
      booleanString = p->value();
      booleanString.toLowerCase();      
      if ( booleanString == "true"){
        AlpacaData.switches.state = true;
      } else {
        AlpacaData.switches.state = false;
      }
    } 
    if (parameter == "brightness") {
      AlpacaData.coverC.brightness = p->value().toInt();
    }       
    
  }
}

void AscomMethodNotImplemented(AsyncWebServerRequest *request) {
  GetAlpArguments(request);
  AsyncResponseStream *response = request->beginResponseStream("application/json");
  response->printf("{%s%d,%s%d,",
                                Alp_CliTraId,AlpacaData.clientTransactionID,
                                Alp_SerTraId,AlpacaData.serverTransactionID
                                );
  response->print(F("\"ErrorNumber\":1024,\"ErrorMessage\":\"Method not implemented\""));
  response->print(F("}"));
  request->send(response); 
}

void AscomPropertyNotImplemented(AsyncWebServerRequest *request) {
  GetAlpArguments(request);
  AsyncResponseStream *response = request->beginResponseStream("application/json");
  response->printf("{%s%d,%s%d,",
                                Alp_CliTraId,AlpacaData.clientTransactionID,
                                Alp_SerTraId,AlpacaData.serverTransactionID
                                );
  response->print(F("\"ErrorNumber\":1024,\"ErrorMessage\":\"Property not implemented\""));
  response->print(F("}"));
  request->send(response); 
}

void AscomNoActions(AsyncWebServerRequest *request){
  GetAlpArguments(request);
  AsyncResponseStream *response = request->beginResponseStream("application/json");
  response->printf("{%s%d,%s%d,%s",
                                Alp_CliTraId,AlpacaData.clientTransactionID,
                                Alp_SerTraId,AlpacaData.serverTransactionID,
                                Alp_NoErrors
                                );
  response->print(F(",\"Value\":[]"));
  response->print(F("}"));
  request->send(response); 
}

void AlpacaManager(){

  Alpserver.on("/management/apiversions",                HTTP_GET, [](AsyncWebServerRequest *request) {
    GetAlpArguments(request);
    AsyncResponseStream *response = request->beginResponseStream("application/json");
    response->print(F("{\"Value\":[1],"));
  response->printf("%s%d,%s%d",
                                Alp_CliTraId,AlpacaData.clientTransactionID,
                                Alp_SerTraId,AlpacaData.serverTransactionID
                                );
    response->print(F("}"));
    request->send(response);
  });

  Alpserver.on("/management/v1/description",                HTTP_GET, [](AsyncWebServerRequest *request) {
    AsyncResponseStream *response = request->beginResponseStream("application/json");
    GetAlpArguments(request);
    response->print(F("{\"Value\": {\"ServerName\": \"StefanoTesla Alpaca Device\",\"Manufacturer\":\"The Stefano Tesla Company\",\"ManufacturerVersion\":\"v2.0.0\",\"Location\":\"Empoli, IT\"},"));
  response->printf("%s%d,%s%d}",
                                Alp_CliTraId,AlpacaData.clientTransactionID,
                                Alp_SerTraId,AlpacaData.serverTransactionID
                                );
    request->send(response);
  });

  Alpserver.on("/management/v1/configureddevices",          HTTP_GET, [](AsyncWebServerRequest *request) {
    AsyncResponseStream *response = request->beginResponseStream("application/json");
    GetAlpArguments(request);
    response->print(F("{\"Value\":["));
    response->print(F("{\"DeviceName\": \"TeslaDome\",\"DeviceType\": \"Dome\",\"DeviceNumber\": 0,\"UniqueID\":\"e989c9b6-ba0f-4834-b299-79a629f2ee59\"},"));
    response->print(F("{\"DeviceName\":\"TeslaSwitch\",\"DeviceType\":\"Switch\",\"DeviceNumber\":0,\"UniqueID\":\"d93f20fb-aa85-49ed-8799-9f50c0969ede\"},"));
    response->print(F("{\"DeviceName\":\"TeslaCoverCalibratior\",\"DeviceType\":\"CoverCalibrator\",\"DeviceNumber\":0,\"UniqueID\":\"35672690-40bf-4165-b44e-d59c2c524f11\"}"));
    response->print(F("],"));
    response->printf("%s%d,%s%d",
      Alp_CliTraId,AlpacaData.clientTransactionID,
      Alp_SerTraId,AlpacaData.serverTransactionID
      );
    response->print(F("}"));
    request->send(response);
  });

}

#endif
