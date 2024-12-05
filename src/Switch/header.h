#ifndef SWITCH_VARIABLE
#define SWITCH_VARIABLE


#define _MAX_SWITCH_ID_ 16

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

struct switchSaveConfigStruct{
  bool execute = false;
  bool failed = false;
  bool restartNeeded = false;
};

struct switchLoadConfigStruct{
  bool isValid = false;
};


struct SwitchConfiStruct{
  unsigned int configuredSwitch;
  switchSaveConfigStruct save;
  switchLoadConfigStruct load;
};


/* SINGLE SWITCH */

enum SwitchType{
  SwTypeNull,
  SwTypeDInput,
  SwTypeAInput,
  SwTypeDOutput,
  SwTypePWM,
  SwTypeServo,
  SwTypeAOutput
};

struct SwitchCommandStruct {
  bool execute;
  bool boValue;
  int intValue;
};

struct SwitchProperty{
  SwitchType type;
  int minValue = 0;
  int maxValue = 1;
  int pwmch;
  unsigned int pin;
};

struct SwitchActualValue{
  int boValue;  //must be true also if is an int value value > minValue, otherwise false
  int intValue; //must return the integer value of the switch, 1 is return if bool is true
};

struct SwitchArrayStruct
{
  String Name;
  String Description;
  SwitchProperty property;
  SwitchCommandStruct command; 
  SwitchActualValue actualValue;
};


/* SWITCH STRUCT */
struct SwitchStruct{
  switchAlpacaParameters alpaca;
  SwitchConfiStruct config;
  SwitchArrayStruct data[_MAX_SWITCH_ID_];
} Switch;


#endif