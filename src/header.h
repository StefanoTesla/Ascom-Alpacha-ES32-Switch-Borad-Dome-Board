
unsigned int pwmchannles = 2;//channel 0 is reserved to coverCalibrator



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

struct globalVariable{
  unsigned long actualMillis;
  unsigned long upTimeInterval = 60000;
  esp32Struct esp32;
  wifiStruct wifi;
};

globalVariable Global;


/* END OF GLOBAL */

