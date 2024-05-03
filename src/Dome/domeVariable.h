#ifndef DOME_VARIABLE
#define DOME_VARIABLE


/* BASE AREA */

enum ShInEnum {
  ShInNoOne,
  ShOnlyClose,
  ShOnlyOpen,
  ShInAll,
};
enum ShStEnum {
  ShOpen,
  ShClose,
  ShOpening,
  ShClosing,
  ShError
};
enum ShCmdEnum {
  Idle,
  CmdOpen,
  CmdClose,
  CmdHalt
};

struct DomeStruct{
  ShCmdEnum ShutterCommand;
  ShStEnum ShutterState;
  ShInEnum ShutterInputState;
  int Cycle;
  bool MoveRetry;
  unsigned int LastDomeCommand =0;
};

DomeStruct Dome;

/* ALPACA AREA */

struct domeAlpacaStruct{
  ShCmdEnum ShutterCommand;
  ShStEnum ShutterState;
  ShInEnum ShutterInputState;
};

/* CONFIG AREA */

struct domeConfig{
  unsigned int pinStart;
  unsigned int pinHalt;
  unsigned int pinOpen;
  unsigned int pinClose;
  unsigned int movingTimeOut = 20;
  bool enAutoClose;
  unsigned int autoCloseTimeOut = 20;
};

struct domeSaveConfigStruct{
  bool execute = false;
  bool failed = false;
  bool restartNeeded = false;
};

struct domeLoadConfigStruct{
  bool isValid = false;
};

#endif