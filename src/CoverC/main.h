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

void setServoAngle(int angle,unsigned int time) {
  //logMessageFormatted(coverc,lInfo,"Cover moved to: %d", angle);
  int dutyMicros = map(angle, 0, CoverC.config.cover.maxDeg, 544, 2500);
  int dutyValue = map(dutyMicros, 0, 20000, 0, 4095);
  ledc_channel_t ledcChannel = static_cast<ledc_channel_t>(0);
  esp_err_t fade_in_status = ledc_set_fade_with_time(LEDC_LOW_SPEED_MODE, ledcChannel, dutyValue, time);
  if (fade_in_status == ESP_OK){
    ledc_fade_start(LEDC_LOW_SPEED_MODE, ledcChannel, LEDC_FADE_NO_WAIT);
  } else {
    Serial.println("ERRORE");
  }
  
  //ledcWrite(CoverC.config.cover.pwmChannel, dutyValue);
}


int dutyToAngle(int duty) {
  float angle = ((float)(duty - 111) / (511 - 111)) * CoverC.config.cover.maxDeg;
  return round(angle);  
}

void coverCycle(){
  int increment;
  CoverC.status.cover.angle = dutyToAngle(ledc_get_duty(LEDC_LOW_SPEED_MODE, static_cast<ledc_channel_t>(0)));
      if(Global.pulse.second.pulse){
    
  logMessageFormatted(coverc,lInfo,"Servo is at: %d the status is: %d", CoverC.status.cover.angle, CoverC.status.cover.cycle);
      }

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
      setServoAngle(CoverC.command.cover.angle,100);
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
      logMessageFormatted(coverc,lInfo,"Servo is going to: %d with time: %d", CoverC.command.cover.angle, CoverC.config.cover.movingTime);
      setServoAngle(CoverC.command.cover.angle,CoverC.config.cover.movingTime);
      CoverC.status.cover.cycle = 21;
      break;

    case 21:
      if((Global.actualMillis - CoverC.command.cover.ackMillis) > CoverC.config.cover.movingTime){
        logMessage(coverc,lInfo,"Cy:11 Moviment finish");
        CoverC.command.cover.move = false;
        CoverC.status.cover.cycle = 0;
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