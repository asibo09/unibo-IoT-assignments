#ifndef __DOORTASK_H__
#define __DOORTASK_H__

#include "../devices/HangarDoor.h"
#include "../kernel/Task.h"


class DoorTask : public Task {
  HangarDoor *door;
  bool isOpen;

public:
  DoorTask();
  void init(int period);
  void tick();
};

#endif