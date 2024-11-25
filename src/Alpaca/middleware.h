#ifndef ALPACA_MIDDLEWARE
#define ALPACA_MIDDLEWARE



AsyncMiddlewareFunction getAlpacaID([](AsyncWebServerRequest* request, ArMiddlewareNext next) {
  int paramsNr = request->params();
  String parameter;
  AlpacaData.serverTransactionID++;
  AlpacaData.clientID = 0;
  AlpacaData.clientTransactionID = 0;

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
  }
  
  next();
});

#endif