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

void setServoAngle(int angle) {
  // Map l'angolo in microsecondi (tempo duty cycle)
  int dutyMicros = map(angle, CoverC.config.cover.closeDeg, CoverC.config.cover.openDeg, 544, 2500);
  int dutyValue = map(dutyMicros, 0, 20000, 0, 255); // 20000µs = periodo 50Hz
  ledcWrite(8, dutyValue);
}

void coverHandlerloop() {

    if(!CoverC.config.cover.present){
      CoverC.status.cover.status = CoverStatusNoPresent;
      return;
    } else {
      CoverC.status.actualBrightness = ledcRead(0);
      if(ledcRead(8) == 544){
        CoverC.status.cover.status = CoverStatusClose;
      } else {
        CoverC.status.cover.status = CoverStatusOpen;
      }
    }

    if(CoverC.command.cover.move){
      Serial.print("vado a: ");
      Serial.print(CoverC.command.cover.angle);
      setServoAngle(CoverC.command.cover.angle);
      CoverC.command.cover.move = false;
    }
}



void coverCalibratorLoop(){
  calibratorhandlerloop();
  coverHandlerloop();

  if (CoverC.config.save.execute){
    saveCoverCConfig();
  }
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