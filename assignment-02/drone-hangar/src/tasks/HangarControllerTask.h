#ifndef __HANGARCONTROLLERTASK_H__
#define __HANGARCONTROLLERTASK_H__

#include "../devices/HangarDoor.h"
#include "../devices/Led.h"
#include "../devices/Pir.h"
#include "../devices/Sonar.h"
#include "../kernel/Task.h"
#include "BlinkTask.h"
#include <LiquidCrystal_I2C.h>

class HangarControllerTask : public Task {
  enum State {
    DRONE_INSIDE,
    TAKE_OFF,
    DRONE_OUT,
    WAIT_FOR_DRONE,
    LANDING,
    ALARM_MODE
  };
  State state;
  State savedState;

  HangarDoor *door;
  ProximitySensor *sonar;
  Pir *pir;
  Light *l1;
  BlinkTask *blinker;
  LiquidCrystal_I2C *lcd;

  int ticksCount;

  // comandi da PC
  bool takeOffCmd;
  bool landingCmd;

  bool preAlarmActive;

  void updateLCD(const char *message);

public:
  HangarControllerTask(BlinkTask *blinkerTask);
  void init(int period);
  void tick();

  // comandi decollo/atterraggio
  void commandTakeOff();
  void commandLanding();

  // comandi per fsm temp
  void setPreAlarm(bool active);
  void triggerAlarm();
  void resetAlarm();
};

#endif