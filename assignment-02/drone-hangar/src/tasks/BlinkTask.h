#ifndef __BLINKTASK__
#define __BLINKTASK__

#include "../devices/Led.h"
#include "../kernel/Task.h"

class BlinkTask : public Task {
  Light *led;
  bool isBlinking;
  int state;

public:
  BlinkTask(int pin);
  void init(int period);
  void tick();
  void setBlinking(bool blink); // accendere/spegnere lampeggio
};

#endif