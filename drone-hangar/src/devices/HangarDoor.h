#ifndef __HANGARDOOR_H__
#define __HANGARDOOR_H__

#include "ServoMotorImpl.h"

class HangarDoor {
  ServoMotor *motor;

public:
  HangarDoor(int pin);
  void open();
  void close();
};

#endif