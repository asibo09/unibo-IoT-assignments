#ifndef __WCS_CONTROLLER_TASK_H__
#define __WCS_CONTROLLER_TASK_H__

#include "../devices/ButtonImpl.h"
#include "../devices/Pot.h"
#include "../devices/ServoMotorImpl.h"
#include "../kernel/Task.h"
#include <LiquidCrystal_I2C.h>

class WcsControllerTask : public Task {
  enum State { AUTOMATIC, MANUAL, UNCONNECTED };
  State state;

  Button *btnMode;
  Potentiometer *pot;
  ServoMotor *servo;
  LiquidCrystal_I2C *lcd;

  int currentValveLevel;
  bool lastBtnState;

  void updateLCD();
  void setValve(int percentage);

public:
  WcsControllerTask();
  void init(int period);
  void tick();
};

#endif