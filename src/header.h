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

struct wifiIpStruct{
  bool enable = false;
  unsigned int ip[4];
  unsigned int sub[4];
};

struct wifiStruct{
  wifiReconntectionStruct reconnection;
  upTimeStruct upTime;
  wifiIpStruct ip;
};

struct esp32Struct{
  upTimeStruct upTime;
};

struct localeStruct{
  String locale = "en";
};


struct boarcConfigStruct{
  wifiStruct wifi;
  esp32Struct esp32;
  localeStruct language;
  boardSaveConfigStruct save;
};

enum ledcType{
  notAssigned,
  pwm,
  servo
};

struct PWMChannelStruct{
  ledcType type;
};


struct globalVariable{
  unsigned long actualMillis;
  unsigned long upTimeInterval = 60000;
  boarcConfigStruct config;
  PWMChannelStruct pwm[16];
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

unsigned int setupLedcChannel(unsigned int channel, ledcType type){

  switch (type)
  {
  case pwm:
    if(ledcSetup(channel, 19531, 12) > 0){
      return channel;
    } else {
      Serial.println("[ERR] INIT: Error during pwm cahnnel setup");
    }
    break;
  case servo:
    if(ledcSetup(channel, 50, 12) > 0){
      return channel;
    } else {
      Serial.println("[ERR] INIT: Error during pwm cahnnel setup");
    }
    break;
  
  default:
      return -1;
    break;
  }

  return -1;
}

unsigned int checkForFreeLedChannel(ledcType type){

  if (type == notAssigned){
    return -1;
  }

  if(type == pwm){
    for (int i = 0; i < 16; i++)
    {
      if(usableLedChannel(i,type)){ return i;}
    }
  } else if (type == servo) {

    //since servo are at 50hz we give the precedence to low speed timer
    for (int i = 8; i < 16; i++)
    {
      if(usableLedChannel(i,type)){ return i;}
    }

    for (int i = 0; i < 8; i++)
    {
      if(usableLedChannel(i,type)){ return i;}
    }

  }
  return -1;
}

unsigned int assignLedChannel(ledcType type){
  unsigned int channel = checkForFreeLedChannel(type);
  Serial.println(channel);
  if (channel < 0 ){
    Serial.println("[ERR] LEDC: error during the search of a free channel.");
    return -1;
  }
  Global.pwm[channel].type = type;
  setupLedcChannel(channel,type);
  return channel;

    
  return -1;


}

void printLEDChannelStatus(){


  Serial.println("ch | type       |");
  Serial.println("------------------");
  for (int i = 0; i < 16; i++)
  {
    if(i<10){
      Serial.print(i);
      Serial.print("  |");
      Serial.print(" ");
      switch (Global.pwm[i].type)
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
        Serial.println("unassigned |");
        break;
      case pwm:
        Serial.println("pwm |");
        break;
      case servo:
        Serial.println("servo |");
        break;
      
      default:
        break;
      }

    }
  }
  
}