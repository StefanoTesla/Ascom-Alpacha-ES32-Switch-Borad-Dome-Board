#ifndef SWITCH_VARIABLE
#define SWITCH_VARIABLE

/* BASE AREA */

enum SwitchType{
  SwTypeDInput,
  SwTypeAInput,
  SwTypeDOutput,
  SwTypePWM,
  SwTypeServo,
};

struct SwitchProperty{
  int minValue = 0;
  int maxValue = 1;
  int Step = 1;
  int type;
  int pwmch;
};

struct SwitchStruct
{
  String Name;
  String Description;
  int pin = 0;
  int cmdValue = -1;
  int readValue;
  SwitchProperty property;
  int pwmChannel = -1;
  
};

#define _MAX_SWITCH_ID_ 16

SwitchStruct Switch[_MAX_SWITCH_ID_];

/* ALPACA AREA */

struct switchAlpacaParameters{
  bool idExist;
  int id;               /* used for switch ID */
  bool stateExist;
  bool state;               /* used for setswitch */
  bool intValueExist;
  int intValue;            /* used for setswitchvalue */
  bool nameExist;
  String name;              /* used for set switch name */
};

/* CONFIGURATION AREA */
struct switchConfig{
  unsigned int configuredSwitch;
};

struct switchSaveConfigStruct{
  bool execute = false;
  bool failed = false;
  bool restartNeeded = false;
};

struct switchLoadConfigStruct{
  bool isValid = false;
};



#endif