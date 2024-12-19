#ifndef CC_MAIN
#define CC_MAIN

#include "config.h"

void calibratorhandlerloop() {

    if(!CoverC.config.calibrator.present){
      CoverC.status.calibrator.status = CalibStatusNoPresent; 
    } else {
      CoverC.status.calibrator.actualBrightness = ledcRead(CoverC.config.calibrator.pwmChannel);
      if(ledcRead(CoverC.config.calibrator.pwmChannel) == 0){
        CoverC.status.calibrator.status = CalibStatusOff;
      } else {
        CoverC.status.calibrator.status = CalibStatusReady;
      }
    }

    if(CoverC.command.calibrator.change){
        logMessage("Calibrator brightness change request",2,2);
        CoverC.command.calibrator.change = false;
        Serial.println(CoverC.command.calibrator.brightness);
        ledcWrite(CoverC.config.calibrator.pwmChannel,CoverC.command.calibrator.brightness);
    }
}

void setServoAngle(int angle) {
  logMessage("Cover movement request",2,2);
  int dutyMicros = map(angle, 0, CoverC.config.cover.maxDeg, 544, 2500);
  int dutyValue = map(dutyMicros, 0, 20000, 0, 4095); 
  ledcWrite(CoverC.config.cover.pwmChannel, dutyValue);
}


int dutyToAngle(int duty) {
  float angle = ((float)(duty - 111) / (511 - 111)) * CoverC.config.cover.maxDeg;
  return round(angle);  
}



void coverCycle(){
  CoverC.status.cover.angle = dutyToAngle(ledcRead(CoverC.config.cover.pwmChannel));
    
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