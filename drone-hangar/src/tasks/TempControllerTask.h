#ifndef __TEMPMONITORTASK_H__
#define __TEMPMONITORTASK_H__

#include "../devices/ButtonImpl.h"
#include "../devices/Led.h"
#include "../devices/TempSensor.h"
#include "../kernel/Task.h"
#include "HangarControllerTask.h"

class TempControllerTask : public Task {
  TempSensor *tempSensor;
  Button *btnReset;
  Led *l3;
  HangarControllerTask *hangarTask;

  enum TempState { NORMAL, CHECK_PRE, PRE_ALARM, CHECK_ALARM, ALARM };
  TempState state;
  int ticksCount;

public:
  TempControllerTask(HangarControllerTask *hangar);
  void init(int period);
  void tick();
};

#endif