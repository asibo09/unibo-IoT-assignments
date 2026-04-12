#ifndef __TEMP_TASK_H__
#define __TEMP_TASK_H__

#include "../devices/TempSensor.h"
#include "../kernel/Task.h"

class TempTask : public Task {
  TempSensor *tempSensor;
  float lastTemp;

public:
  TempTask();
  void init(int period);
  void tick();
};

#endif
