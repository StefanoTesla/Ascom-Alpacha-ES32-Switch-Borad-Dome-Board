#ifndef CC_VARIABLE
#define CC_VARIABLE

/* CONFIG*/

struct coverCSaveConfigStruct{
  bool execute = false;
  bool failed = false;
  bool restartNeeded = false;
};

struct coverCLoadConfigStruct{
  bool isValid = false;
};

struct calibratorConfig{
  bool present;
  unsigned int outPWM;
};

struct coverConfig{
  bool present;
  unsigned int outServoPin;
  unsigned int openDeg;
  unsigned int closeDeg = 0;
  unsigned int maxDeg = 0;
  unsigned int movingTime = 2000;
};

struct structCoverCalibratorConfig{
  calibratorConfig calibrator;
  coverConfig cover;
  coverCSaveConfigStruct save;
  coverCLoadConfigStruct load;
};

/* COMMANDS */
struct calibratorCommand{
  bool change;
  unsigned int brightness; 
};

struct coverCommand{
  bool move;
  unsigned int angle; 
};

struct coverCalibratorsCommand{
  calibratorCommand calibrator;
  coverCommand cover;
};


/* STATUS*/
enum calirbatorStatusEnum{
  CalibStatusNoPresent,
  CalibStatusOff,
  CalibStatusNotReady,
  CalibStatusReady,
  CalibStatusUnknow,
  CalibStatusError
};

enum coverStatusEnum{
  CoverStatusNoPresent,
  CoverStatusClose,
  CoverStatusMoving,
  CoverStatusOpen,
  CoverStatusUnknow,
  CoverStatusError
};

struct coverStatus{
  coverStatusEnum status;
  unsigned int cycle;
  unsigned int angle;
};

struct coverStruct {
  coverStatus status;
  coverCommand command;
};

struct coverCalibratorStatus{
  calirbatorStatusEnum calibrator;
  coverStatus cover;
  unsigned int actualBrightness;
};

/* ALPACA*/

struct alpacaCalibratorParameters{
  bool exist;
  unsigned int brightness;
};

struct coverCalibrator{
  coverCalibratorStatus status;
  coverCalibratorsCommand command;
  structCoverCalibratorConfig config;
  alpacaCalibratorParameters alpaca;
};

coverCalibrator CoverC;


#endif