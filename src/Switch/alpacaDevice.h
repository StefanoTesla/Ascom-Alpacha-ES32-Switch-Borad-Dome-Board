#ifndef SWITCH_ALPACA_DEVICE
#define SWITCH_ALPACA_DEVICE
#include "libraries.h"
#include "header.h"

AsyncMiddlewareFunction getID([](AsyncWebServerRequest* request, ArMiddlewareNext next) {
      int paramsNr = request->params();
      String parameter;
      Switch.alpaca.idExist = false;
      int id = -1;
      for (int i = 0; i < paramsNr; i++) {
            const AsyncWebParameter* p = request->getParam(i);
            parameter = p->name();
            parameter.toLowerCase();
            if (parameter == "id") {
                  id = p->value().toInt();
                  if( id >= 0 && id < _MAX_SWITCH_ID_){
                        Switch.alpaca.idExist = true;
                        Switch.alpaca.id = p->value().toInt();
                  }
                  next();
            }
      }
      next();
});

AsyncMiddlewareFunction getValue([](AsyncWebServerRequest* request, ArMiddlewareNext next) {
      int paramsNr = request->params();
      String parameter;
      Switch.alpaca.intValueExist = false;

      for (int i = 0; i < paramsNr; i++) {
            const AsyncWebParameter* p = request->getParam(i);
            parameter = p->name();
            parameter.toLowerCase();
            if (parameter == "value") {
                  Switch.alpaca.intValueExist = true;
                  Switch.alpaca.intValue = p->value().toInt();
                  next();
            }
      }
      next();
});

void switchAlpacaDevice(){

      alpaca.on("/api/v1/switch/0/maxswitch", HTTP_GET, [](AsyncWebServerRequest *request){
            AsyncJsonResponse* response = new AsyncJsonResponse();
            JsonObject doc = response->getRoot().to<JsonObject>();
            doc["Value"] = Switch.config.configuredSwitch;
            doc["ErrorNumber"] = 0;
            doc["ErrorMessage"] = "";
            doc["ClientTransactionID"] = AlpacaData.clientTransactionID;
            doc["ServerTransactionID"] = AlpacaData.serverTransactionID;
            response->setLength();
            request->send(response);
      }).addMiddleware(&getAlpacaID);

      alpaca.on("/api/v1/switch/0/canwrite", HTTP_GET, [](AsyncWebServerRequest *request){
            AsyncJsonResponse* response = new AsyncJsonResponse();
            JsonObject doc = response->getRoot().to<JsonObject>();

            if(Switch.alpaca.idExist){
                  bool canwrite = false;
                  if(
                        Switch.data[Switch.alpaca.id].property.type == SwTypeAOutput ||
                        Switch.data[Switch.alpaca.id].property.type == SwTypeDOutput ||
                        Switch.data[Switch.alpaca.id].property.type == SwTypePWM ||
                        Switch.data[Switch.alpaca.id].property.type == SwTypeServo
                  ){ canwrite = true; }

                  doc["Value"] = canwrite;
                  doc["ErrorNumber"] = 0;
                  doc["ErrorMessage"] = "";
            } else {
                  doc["ErrorNumber"] = 1025;
                  doc["ErrorMessage"] = "ID not provided or outside range.";
            }

            doc["ClientTransactionID"] = AlpacaData.clientTransactionID;
            doc["ServerTransactionID"] = AlpacaData.serverTransactionID;
            response->setLength();
            request->send(response);
  }).addMiddlewares({&getAlpacaID,&getID});

  alpaca.on("/api/v1/switch/0/getswitch", HTTP_GET, [](AsyncWebServerRequest *request){
        AsyncJsonResponse* response = new AsyncJsonResponse();
        JsonObject doc = response->getRoot().to<JsonObject>();
        doc["Value"] = CoverC.status.calibrator.status;
        doc["ErrorNumber"] = 0;
        doc["ErrorMessage"] = "";
        doc["ClientTransactionID"] = AlpacaData.clientTransactionID;
        doc["ServerTransactionID"] = AlpacaData.serverTransactionID;
        response->setLength();
        request->send(response);
  }).addMiddleware(&getAlpacaID);

  alpaca.on("/api/v1/covercalibrator/0/covermoving", HTTP_GET, [](AsyncWebServerRequest *request){
        AsyncJsonResponse* response = new AsyncJsonResponse();
        JsonObject doc = response->getRoot().to<JsonObject>();
        doc["Value"] = CoverC.status.cover.status == CoverStatusMoving ? true : false;
        doc["ErrorNumber"] = 0;
        doc["ErrorMessage"] = "";
        doc["ClientTransactionID"] = AlpacaData.clientTransactionID;
        doc["ServerTransactionID"] = AlpacaData.serverTransactionID;
        response->setLength();
        request->send(response);
  }).addMiddleware(&getAlpacaID);

  alpaca.on("/api/v1/covercalibrator/0/coverstate", HTTP_GET, [](AsyncWebServerRequest *request){
        AsyncJsonResponse* response = new AsyncJsonResponse();
        JsonObject doc = response->getRoot().to<JsonObject>();
        doc["Value"] = CoverC.status.cover.status;
        doc["ErrorNumber"] = 0;
        doc["ErrorMessage"] = "";
        doc["ClientTransactionID"] = AlpacaData.clientTransactionID;
        doc["ServerTransactionID"] = AlpacaData.serverTransactionID;
        response->setLength();
        request->send(response);
  }).addMiddleware(&getAlpacaID);

  alpaca.on("/api/v1/covercalibrator/0/maxbrightness", HTTP_GET, [](AsyncWebServerRequest *request){
        AsyncJsonResponse* response = new AsyncJsonResponse();
        JsonObject doc = response->getRoot().to<JsonObject>();
        doc["Value"] = 4096;
        doc["ErrorNumber"] = 0;
        doc["ErrorMessage"] = "";
        doc["ClientTransactionID"] = AlpacaData.clientTransactionID;
        doc["ServerTransactionID"] = AlpacaData.serverTransactionID;
        response->setLength();
        request->send(response);
  }).addMiddleware(&getAlpacaID);

  alpaca.on("/api/v1/covercalibrator/0/devicestate", HTTP_GET, [](AsyncWebServerRequest *request) {
    AsyncJsonResponse* response = new AsyncJsonResponse();
    JsonObject doc = response->getRoot().to<JsonObject>();
    JsonArray Value = doc["Value"].to<JsonArray>();
    JsonObject calibStatus = Value.add<JsonObject>();
    calibStatus["Name"] = "CalibratorState";
    calibStatus["Value"] = CoverC.status.calibrator.status;
    JsonObject calibChanging = Value.add<JsonObject>();
    calibChanging["Name"] = "CalibratorChanging";
    calibChanging["Value"] = false;
    JsonObject coverState = Value.add<JsonObject>();
    coverState["Name"] = "CoverState";
    coverState["Value"] = CoverC.status.cover.status;
    JsonObject coverMoving = Value.add<JsonObject>();
    coverMoving["Name"] = "CoverMoving";
    coverMoving["Value"] = false;
    doc["ErrorNumber"] = 0;
    doc["ErrorMessage"] = "";
    doc["ClientTransactionID"] = AlpacaData.clientTransactionID;
    doc["ServerTransactionID"] = AlpacaData.serverTransactionID;
    response->setLength();
    request->send(response);
}).addMiddleware(&getAlpacaID);

  alpaca.on("/api/v1/covercalibrator/0/calibratoroff", HTTP_PUT, [](AsyncWebServerRequest *request){
      AsyncJsonResponse* response = new AsyncJsonResponse();
      JsonObject doc = response->getRoot().to<JsonObject>();
      doc["ClientTransactionID"] = AlpacaData.clientTransactionID;
      doc["ServerTransactionID"] = AlpacaData.serverTransactionID;
      if(!CoverC.config.calibrator.present){
            doc["ErrorNumber"] = 1024;
            doc["ErrorMessage"] = "Method not implemented";
      } else if(!CoverC.alpaca.exist){
            doc["ErrorNumber"] = 1025;
            doc["ErrorMessage"] = "Brightness parameter not found";
      } else{
            CoverC.command.calibrator.change = true;
            CoverC.command.calibrator.brightness = 0;
            doc["ErrorNumber"] = 0;
            doc["ErrorMessage"] = "";
      }
      
      response->setLength();
      request->send(response); 
  }).addMiddleware(&getAlpacaID);

  alpaca.on("/api/v1/covercalibrator/0/closecover", HTTP_PUT, [](AsyncWebServerRequest *request){
      AsyncJsonResponse* response = new AsyncJsonResponse();
      JsonObject doc = response->getRoot().to<JsonObject>();
      doc["ClientTransactionID"] = AlpacaData.clientTransactionID;
      doc["ServerTransactionID"] = AlpacaData.serverTransactionID;
      if(!CoverC.config.cover.present){
            doc["ErrorNumber"] = 1024;
            doc["ErrorMessage"] = "Method not implemented";
      } else if(CoverC.status.cover.status == 2){
            doc["ErrorNumber"] = 1035;
            doc["ErrorMessage"] = "Cover is moving";
      } else{
            Serial.println("ascom is goin to close");
            CoverC.command.cover.move = true;
            CoverC.command.cover.angle = CoverC.config.cover.closeDeg;
            doc["ErrorNumber"] = 0;
            doc["ErrorMessage"] = "";
      }
      
      response->setLength();
      request->send(response); 
  }).addMiddleware(&getAlpacaID);

  alpaca.on("/api/v1/covercalibrator/0/opencover", HTTP_PUT, [](AsyncWebServerRequest *request){
      AsyncJsonResponse* response = new AsyncJsonResponse();
      JsonObject doc = response->getRoot().to<JsonObject>();
      doc["ClientTransactionID"] = AlpacaData.clientTransactionID;
      doc["ServerTransactionID"] = AlpacaData.serverTransactionID;
      if(!CoverC.config.cover.present){
            doc["ErrorNumber"] = 1024;
            doc["ErrorMessage"] = "Method not implemented";
      } else if(CoverC.status.cover.status == 2){
            doc["ErrorNumber"] = 1035;
            doc["ErrorMessage"] = "Cover is moving";
      } else{
            Serial.println("ascom is goin to close");
            CoverC.command.cover.move = true;
            CoverC.command.cover.angle = CoverC.config.cover.openDeg;
            doc["ErrorNumber"] = 0;
            doc["ErrorMessage"] = "";
      }
      
      response->setLength();
      request->send(response); 
  }).addMiddleware(&getAlpacaID);

  alpaca.on("/api/v1/covercalibrator/0/calibratoron", HTTP_PUT, [](AsyncWebServerRequest *request){
      AsyncJsonResponse* response = new AsyncJsonResponse();
      JsonObject doc = response->getRoot().to<JsonObject>();
      doc["ClientTransactionID"] = AlpacaData.clientTransactionID;
      doc["ServerTransactionID"] = AlpacaData.serverTransactionID;
      if(!CoverC.config.calibrator.present){
            doc["ErrorNumber"] = 1024;
            doc["ErrorMessage"] = "Method not implemented";
      } else if(!CoverC.alpaca.exist){
            doc["ErrorNumber"] = 1025;
            doc["ErrorMessage"] = "Brightness parameter not found";
      } else if(!CoverC.alpaca.brightness < 0 or !CoverC.alpaca.brightness > 100){
            doc["ErrorNumber"] = 1025;
            doc["ErrorMessage"] = "Value outside MIN and MAX";
      } else {
            CoverC.command.calibrator.change = true;
            CoverC.command.calibrator.brightness = CoverC.alpaca.brightness;
            doc["ErrorNumber"] = 0;
            doc["ErrorMessage"] = "";
      }
      response->setLength();
      request->send(response);
  }).addMiddlewares({&getAlpacaID,&getBrightness});

    /* Property not implemented:*/
    alpaca.on("/api/v1/covercalibrator/0/haltcover",        HTTP_PUT, alpacaPropertyNotImplemented).addMiddleware(&getAlpacaID);


    /* Methods not implemented:*/
    alpaca.on("/api/v1/covercalibrator/0/commandblind",     HTTP_PUT, alpacaMethodNotImplemented).addMiddleware(&getAlpacaID);
    alpaca.on("/api/v1/covercalibrator/0/commandbool",      HTTP_PUT, alpacaMethodNotImplemented).addMiddleware(&getAlpacaID);
    alpaca.on("/api/v1/covercalibrator/0/commandstring",    HTTP_PUT, alpacaMethodNotImplemented).addMiddleware(&getAlpacaID);

   /* I don't care about connection but we need to declare it*/
    alpaca.on("/api/v1/covercalibrator/0/connect", HTTP_PUT, [](AsyncWebServerRequest *request) {
        AsyncJsonResponse* response = new AsyncJsonResponse();
        JsonObject doc = response->getRoot().to<JsonObject>();
        doc["ErrorNumber"] = 0;
        doc["ErrorMessage"] = "";
        doc["ClientTransactionID"] = AlpacaData.clientTransactionID;
        doc["ServerTransactionID"] = AlpacaData.serverTransactionID;
        response->setLength();
        request->send(response);

    }).addMiddleware(&getAlpacaID);

    alpaca.on("/api/v1/covercalibrator/0/disconnect", HTTP_PUT, [](AsyncWebServerRequest *request) {
        AsyncJsonResponse* response = new AsyncJsonResponse();
        JsonObject doc = response->getRoot().to<JsonObject>();
        doc["ErrorNumber"] = 0;
        doc["ErrorMessage"] = "";
        doc["ClientTransactionID"] = AlpacaData.clientTransactionID;
        doc["ServerTransactionID"] = AlpacaData.serverTransactionID;
        response->setLength();
        request->send(response);

    }).addMiddleware(&getAlpacaID);

    alpaca.on("/api/v1/covercalibrator/0/connecting", HTTP_GET, [](AsyncWebServerRequest *request) {
        AsyncJsonResponse* response = new AsyncJsonResponse();
        JsonObject doc = response->getRoot().to<JsonObject>();
        doc["Value"] = false;
        doc["ErrorNumber"] = 0;
        doc["ErrorMessage"] = "";
        doc["ClientTransactionID"] = AlpacaData.clientTransactionID;
        doc["ServerTransactionID"] = AlpacaData.serverTransactionID;
        response->setLength();
        request->send(response);
    }).addMiddleware(&getAlpacaID);

    alpaca.on("/api/v1/covercalibrator/0/connected", HTTP_GET, [](AsyncWebServerRequest *request) {
        AsyncJsonResponse* response = new AsyncJsonResponse();
        JsonObject doc = response->getRoot().to<JsonObject>();
        doc["Value"] = true;
        doc["ErrorNumber"] = 0;
        doc["ErrorMessage"] = "";
        doc["ClientTransactionID"] = AlpacaData.clientTransactionID;
        doc["ServerTransactionID"] = AlpacaData.serverTransactionID;
        response->setLength();
        request->send(response);
    }).addMiddleware(&getAlpacaID);

    alpaca.on("/api/v1/covercalibrator/0/connected", HTTP_PUT, [](AsyncWebServerRequest *request) {
        AsyncJsonResponse* response = new AsyncJsonResponse();
        JsonObject doc = response->getRoot().to<JsonObject>();
        doc["ErrorNumber"] = 0;
        doc["ErrorMessage"] = "";
        doc["ClientTransactionID"] = AlpacaData.clientTransactionID;
        doc["ServerTransactionID"] = AlpacaData.serverTransactionID;
        response->setLength();
        request->send(response);
    }).addMiddleware(&getAlpacaID);

    alpaca.on("/api/v1/covercalibrator/0/supportedactions",HTTP_GET, alpacaNoActions).addMiddleware(&getAlpacaID);
    alpaca.on("/api/v1/covercalibrator/0/action",HTTP_PUT, alpacaActionNotImplemented).addMiddleware(&getAlpacaID);
}

#endif