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

    if(CoverC.command.calibrator.change){
        CoverC.command.calibrator.change = false;
        Serial.println(CoverC.command.calibrator.brightness);
        ledcWrite(0,CoverC.command.calibrator.brightness);
    }
}

void setServoAngle(int angle) {
  int dutyMicros = map(angle, 0, CoverC.config.cover.maxDeg, 544, 2500);
  int dutyValue = map(dutyMicros, 0, 20000, 0, 4095); 
  ledcWrite(8, dutyValue);
}


int dutyToAngle(int duty) {
  float angle = ((float)(duty - 111) / (511 - 111)) * CoverC.config.cover.maxDeg;
  return round(angle);  
}



void coverCycle(){
  CoverC.status.cover.angle = dutyToAngle(ledcRead(8));
    
  switch (CoverC.status.cover.cycle){
    case 0:
        if (CoverC.status.cover.angle == CoverC.config.cover.closeDeg){
          CoverC.status.cover.status = CoverStatusClose;
        } else if (CoverC.status.cover.angle == CoverC.config.cover.openDeg){
          CoverC.status.cover.status = CoverStatusOpen;
        }
        if( CoverC.status.cover.angle < 0 or CoverC.status.cover.angle > CoverC.config.cover.maxDeg){
          CoverC.status.cover.status = CoverStatusUnknow;
        }
        
        if (CoverC.command.cover.move){
          CoverC.command.cover.ackMillis = Global.actualMillis;
          setServoAngle(CoverC.command.cover.angle);
          CoverC.status.cover.cycle = 10;
        }
        break;
    
    case 10:
      CoverC.status.cover.status = CoverStatusMoving;
      if((Global.actualMillis - CoverC.command.cover.ackMillis) > CoverC.config.cover.movingTime){
        CoverC.command.cover.move = false;
        CoverC.status.cover.cycle = 0;
      }
      break;

    default:
      Serial.println("COVER: CYCLE OVERFLOW");

  }

}

void coverHandlerloop() {

    if(!CoverC.config.cover.present){
      CoverC.status.cover.status = CoverStatusNoPresent;
      return;
    }
    
    coverCycle();
    
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