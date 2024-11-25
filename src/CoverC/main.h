#ifndef CC_MAIN
#define CC_MAIN

#include "config.h"

void calibratorhandlerloop() {

    if(!CoverC.config.calibrator.present){
      CoverC.status.calibrator = CalibStatusNoPresent; 
    } else {
      CoverC.status.actualBrightness = ledcRead(0);
      if(ledcRead(0) == 0){
        CoverC.status.calibrator = CalibStatusOff;
      } else {
        CoverC.status.calibrator = CalibStatusReady;
      }
    }
}

void coverHandlerloop() {

    if(!CoverC.config.cover.present){
      CoverC.status.cover.status = CoverStatusNoPresent; 
    } else {
      CoverC.status.actualBrightness = ledcRead(0);
      if(ledcRead(0) == 0){
        CoverC.status.calibrator = CalibStatusOff;
      } else {
        CoverC.status.calibrator = CalibStatusReady;
      }
    }
}



void coverCalibratorLoop(){
  calibratorhandlerloop();
  coverHandlerloop();
}

#include "webserver.h"
#include "alpacaDevice.h"
#include "alpacaManage.h"

void coverCalibratorRequestHandler(){

  coverAlpacaManage();
  coverAlpacaDevice();
  coverWebServer();
}
#endif