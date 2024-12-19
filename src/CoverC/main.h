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
        logMessageFormatted(coverc,lInfo,"Brightness changed to: %d", CoverC.command.calibrator.brightness);
        CoverC.command.calibrator.change = false;
        Serial.println(CoverC.command.calibrator.brightness);
        ledcWrite(CoverC.config.calibrator.pwmChannel,CoverC.command.calibrator.brightness);
    }
}

void setServoAngle(int angle) {
  //logMessageFormatted(coverc,lInfo,"Cover moved to: %d", angle);
  int dutyMicros = map(angle, 0, CoverC.config.cover.maxDeg, 544, 2500);
  int dutyValue = map(dutyMicros, 0, 20000, 0, 4095); 
  ledcWrite(CoverC.config.cover.pwmChannel, dutyValue);
}


int dutyToAngle(int duty) {
  float angle = ((float)(duty - 111) / (511 - 111)) * CoverC.config.cover.maxDeg;
  return round(angle);  
}

void coverCycle(){
  int increment;
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
      
      if(CoverC.status.cover.status != CoverStatusMoving){
        if (CoverC.command.cover.move){
          CoverC.command.cover.ackMillis = Global.actualMillis;

          if(CoverC.status.cover.status == CoverStatusUnknow){
            CoverC.status.cover.cycle = 10;
            CoverC.status.cover.status = CoverStatusMoving;
          } else {
            CoverC.status.cover.cycle = 20;
            CoverC.status.cover.status = CoverStatusMoving;
          }
        }
      }

      break;
    
    //from unknow angle
    case 10:
      logMessage(coverc,lInfo,"Cy:10 Moving from undefined position");
      CoverC.command.cover.move = false;
      setServoAngle(CoverC.command.cover.angle);
      CoverC.status.cover.cycle = 11;
      break;
    
    case 11:
      if((Global.actualMillis - CoverC.command.cover.ackMillis) > CoverC.config.cover.movingTime){
        logMessage(coverc,lInfo,"Cy:11 Moviment finish");
        CoverC.status.cover.cycle = 0;
      }
      break;

    //inc or dec angle?
    case 20:
      if(CoverC.command.cover.angle > CoverC.status.cover.angle){
        logMessage(coverc,lInfo,"Cy:20 Moving to an higher position");
        CoverC.status.cover.cycle = 30;
      } else if(CoverC.command.cover.angle < CoverC.status.cover.angle ){
        CoverC.status.cover.cycle = 40;
        logMessage(coverc,lInfo,"Cy:20 Moving to a lower position");
      } else {
        logMessage(coverc,lInfo,"Cy:20 Moviment finish");
        CoverC.status.cover.cycle = 0;
      }
      break;

    case 30:
      increment = (CoverC.config.cover.movingTime / (CoverC.command.cover.angle - CoverC.status.cover.angle)) * 5;
      if(increment == 0){ increment = 1;}

      CoverC.command.cover.goToAngle = CoverC.status.cover.angle; 
      Serial.print("30- devo inc ongi: ");
      Serial.println(increment);
      CoverC.status.cover.cycle = 31;
      break;

    case 31:
      Serial.println(Global.actualMillis);
      CoverC.command.cover.ackMillis = Global.actualMillis;
      CoverC.status.cover.cycle = 32;
      break;
    case 32:
      Serial.println("wait");
      if((Global.actualMillis - CoverC.command.cover.ackMillis) >= increment){
        CoverC.command.cover.goToAngle = CoverC.command.cover.goToAngle + 5;
        CoverC.status.cover.cycle = 33;
      }
      break;

    case 33:
      Serial.print("sono a: ");
      Serial.print(CoverC.status.cover.angle);    
      Serial.print(" vado a: ");
      Serial.print(CoverC.command.cover.goToAngle);
      Serial.print(" dv arr a:");
      Serial.println(CoverC.command.cover.angle);

    if(CoverC.command.cover.goToAngle < CoverC.command.cover.angle ){

      setServoAngle(CoverC.command.cover.goToAngle);
      CoverC.status.cover.cycle = 31;
      Serial.println("increment");
    } else {
      Serial.println("arrivato");
      setServoAngle(CoverC.command.cover.angle);
      CoverC.status.cover.cycle = 0;
      CoverC.command.cover.move = false;
      logMessage(coverc,lInfo,"Cy:33 Moviment finish");
    }
      break;

    case 40:
      increment = (CoverC.config.cover.movingTime / (CoverC.status.cover.angle - CoverC.command.cover.angle));
      CoverC.status.cover.cycle = 41;
      break;

    case 41:
      CoverC.command.cover.ackMillis = Global.actualMillis;
      CoverC.status.cover.cycle = 42;
      break;

    case 42:
      if((Global.actualMillis - CoverC.command.cover.ackMillis) >= increment){
        CoverC.status.cover.cycle = 43;
      }
      break;

    case 43:
      if((CoverC.status.cover.angle -1) > CoverC.command.cover.angle ){
          setServoAngle(CoverC.status.cover.angle - 1);
          CoverC.status.cover.cycle = 41;
      } else {
        setServoAngle(CoverC.command.cover.angle);
        CoverC.status.cover.cycle = 0;
        CoverC.command.cover.move = false;
        logMessage(coverc,lInfo,"Cy:43 Moviment finish");
      }
      break;



    default:
      logMessage(coverc,lErr,"Cover cycle OverFlow");
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