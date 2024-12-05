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
                Switch.data[i].actualValue.intValue =digitalRead(Switch.data[i].property.pin);
                break;
            case SwTypeAInput:
            case SwTypeAOutput:
                value = map(analogRead(Switch.data[i].property.pin),0,4095,Switch.data[i].property.minValue,Switch.data[i].property.maxValue);
                Switch.data[i].actualValue.boValue = value > Switch.data[i].property.minValue ? true : false;
                Switch.data[i].actualValue.intValue = value;
                break;
            case SwTypePWM:
                value  = ledcRead(Switch.data[i].property.pwmch);
                Switch.data[i].actualValue.boValue = value > 0 ? true : false;
                Switch.data[i].actualValue.intValue = value;
                break;
            default:
                Serial.println("ma che cazz vuoi?");
                break;
        }

    }
    
}





void SwitchLoop(){
    updateSwitchState();
}