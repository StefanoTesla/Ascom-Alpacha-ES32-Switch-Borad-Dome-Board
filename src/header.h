/* input structure */
struct InputStructure {
  unsigned int pin;
  bool type; //0=NO 1=NC
  unsigned long delayON;
  unsigned long delayOFF;
  unsigned long _ackOn;
  unsigned long _ackOff;
  bool reInput;
  bool feInput;
};

/* ALPACA DATA */

struct AlpacaCommonData{
  uint32_t clientTransactionID;
  uint32_t serverTransactionID = 0;
  uint32_t clientID;
};

AlpacaCommonData AlpacaData;

/*END ALPACA DATA */


/** CONFIG STRUCT **/
struct boardSaveConfigStruct{
  bool execute = false;
  bool failed = false;
  bool restartNeeded = false;
};

/* END OF CONFIG */



/* GLOBAL VARIABLES */
struct upTimeStruct{
  unsigned long previousMillis = 0;
  unsigned long minutes = 0;
};

struct wifiReconntectionStruct{
  unsigned long intervall = 30000;
  unsigned long lastMillis = 0;
  bool waitToReconnect = false;
};

struct wifiStruct{
  wifiReconntectionStruct reconnection;
  upTimeStruct upTime;
};

struct esp32Struct{
  upTimeStruct upTime;
};

enum ledcType{
  notAssigned,
  pwm,
  servo
};

struct PWMChannelStruct{
  ledcType type;
};
struct TimerStruct{
  ledcType type;
};

struct globalVariable{
  unsigned long actualMillis;
  unsigned long upTimeInterval = 60000;
  esp32Struct esp32;
  wifiStruct wifi;
  PWMChannelStruct pwm[16];
  TimerStruct timer[8];
};

globalVariable Global;
/* END OF GLOBAL */

bool usableLedChannel(unsigned int channel,ledcType type){

  if(type == notAssigned){
    Serial.println("[ERR] LEDC: Invalid type");
    return false;
  }

  if(channel > 15){
    Serial.println("[ERR] LEDC: Invalid channel number, maximum is 15");
    return false;
  }

  if(Global.pwm[channel].type != notAssigned){
    return false;
  }

  int x = 0;
  int otherChannel=0;

  float ch = channel / 2.0;
  x = ch;
  float modulo = ch - x;

  if(modulo > 0.0){ 
    otherChannel = channel - 1; 
  } else {
    otherChannel = channel + 1; 
  }

  if(Global.pwm[otherChannel].type != notAssigned && Global.pwm[otherChannel].type != type){
    return false;
  }

  return true;

}

void setupLedCTimer(unsigned int channel, ledcType type){

  unsigned int timer;
  float ch = channel / 2;
  timer = ch;

  switch (type)
  {
  case pwm:
    if(ledcSetup(timer, 19531, 12) > 0){
      Global.timer[timer].type = pwm;
    };
    break;
  case servo:
    if(ledcSetup(timer, 50, 12) > 0){
      Global.timer[timer].type = servo;
    };
    break;
  
  default:
    break;
  }
}

bool assignLedChannel(unsigned int channel,ledcType type){

  if(usableLedChannel(channel,type)){
    Global.pwm[channel].type = type;
    setupLedCTimer(channel,type);
    return true;
  }
    Serial.println("[ERR] LEDC: you can't use this channel, or is already assigned or a different Type is been set.");
  return false;


}

int checkForFreeLedChannel(ledcType type){
  for (int i = 0; i < 16; i++)
  {
    if(usableLedChannel(i,type)){ return i;}
  }

  return -1;
  
}


void printLEDChannelStatus(){


  Serial.println("ch | type       | timer");
  Serial.println("------------------------");
  for (int i = 0; i < 16; i++)
  {
    if(i<10){
      Serial.print(i);
      Serial.print("  |");
      Serial.print(" ");
      switch (Global.pwm[i].type)
      {
      case notAssigned:
        Serial.print("unassigned | ");
        break;
      case pwm:
        Serial.print("pwm        | ");
        break;
      case servo:
        Serial.print("servo      | ");
        break;
      
      default:
        break;
      }

      int timer = i / 2;
      switch (Global.timer[timer].type)
      {
      case notAssigned:
        Serial.println("unassigned | ");
        break;
      case pwm:
        Serial.println("pwm        | ");
        break;
      case servo:
        Serial.println("servo      | ");
        break;
      
      default:
        break;
      }

    } else {

      Serial.print(i);
      Serial.print(" |");
      Serial.print(" ");
      switch (Global.pwm[i].type)
      {
      case notAssigned:
        Serial.print("unassigned |");
        break;
      case pwm:
        Serial.print("pwm |");
        break;
      case servo:
        Serial.print("servo |");
        break;
      
      default:
        break;
      }

      int timer = i / 2;
      switch (Global.timer[timer].type)
      {
      case notAssigned:
        Serial.println(" unassigned | ");
        break;
      case pwm:
        Serial.println(" pwm        | ");
        break;
      case servo:
        Serial.println(" servo      | ");
        break;
      
      default:
        break;
      }
    }
  }
  
}