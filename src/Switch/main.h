#ifndef SWITCH_MAIN
#define SWITCH_MAIN

#include "config.h"

void updateSwitchState(){
    for(int i=0;i<_MAX_SWITCH_ID_;i++){
        int value = 0;
        switch(Switch.data[i].property.type){

            case SwTypeNull:
                Switch.data[i].actualValue.boValue =false;
                Switch.data[i].actualValue.intValue = 0;
                break;
            case SwTypeDInput:
            case SwTypeDOutput:
                digitalRead(Switch.data[i].property.pin);
                Switch.data[i].actualValue.boValue = digitalRead(Switch.data[i].property.pin) ? true : false;
                Switch.data[i].actualValue.intValue = digitalRead(Switch.data[i].property.pin);
                break;
            case SwTypeAInput:
            case SwTypeAOutput:
                value = map(analogRead(Switch.data[i].property.pin),0,4095,Switch.data[i].property.minValue,Switch.data[i].property.maxValue);
                Switch.data[i].actualValue.boValue = value > Switch.data[i].property.minValue ? true : false;
                Switch.data[i].actualValue.intValue = value;
                break;
            case SwTypePWM:
                Switch.data[i].actualValue.intValue = map(ledcRead(Switch.data[i].property.pwmch),0,4096,Switch.data[i].property.minValue,Switch.data[i].property.maxValue);
                Switch.data[i].actualValue.boValue = Switch.data[i].actualValue.intValue > Switch.data[i].property.minValue ? true : false;
                break;
            case SwTypeServo:
                value  = ledcRead(Switch.data[i].property.pwmch);
                break;
            default:
                break;
        }

    }
    
}


void setSwitchValue(){
    for(int i=0;i<_MAX_SWITCH_ID_;i++){
        if(!Switch.data[i].command.execute){
            continue;
        }
        int value;
        switch(Switch.data[i].property.type){

            case SwTypeNull:
                Switch.data[i].command.execute =false;
                Switch.data[i].actualValue.intValue = 0;
                break;
            case SwTypeDOutput:
                digitalWrite(Switch.data[i].property.pin,Switch.data[i].command.boValue);
                break;
        //    case SwTypeAOutput:
        //        value = map(analogRead(Switch.data[i].property.pin),0,4095,Switch.data[i].property.minValue,Switch.data[i].property.maxValue);
        //        Switch.data[i].actualValue.boValue = value > Switch.data[i].property.minValue ? true : false;
        //        Switch.data[i].actualValue.intValue = value;
        //       break;
            case SwTypePWM:
            case SwTypeServo:
                value = map(Switch.data[i].command.intValue,Switch.data[i].property.minValue,Switch.data[i].property.maxValue,0,4096);
                ledcWrite(Switch.data[i].property.pwmch,value);
                break;
            default:
                break;
        }
        Switch.data[i].command.execute = false;

    }

}


void SwitchLoop(){
    updateSwitchState();
    setSwitchValue();


    if(Switch.config.save.execute){
        Switch.config.save.execute = false;
        saveSwitchConfig();
    }
}


#include "webserver.h"

void switchRequestHandler(){
  switchWebServer();
}
#endif