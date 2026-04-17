#ifndef __PIRTASK__
#define __PIRTASK__

#include "../devices/Pir.h"
#include "../kernel/Task.h"

class PirTask : public Task {
  Pir *pir;
  bool lastState;

public:
  PirTask();
  void init(int period);
  void tick();
};

#endif