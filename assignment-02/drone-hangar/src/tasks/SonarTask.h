#ifndef __SONARTASK__
#define __SONARTASK__

#include "../devices/Sonar.h"
#include "../kernel/Task.h"


class SonarTask : public Task {
  ProximitySensor *sonar;

public:
  SonarTask();
  void init(int period);
  void tick();
};

#endif