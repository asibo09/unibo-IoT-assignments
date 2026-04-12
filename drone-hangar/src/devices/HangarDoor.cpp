#include "HangarDoor.h"
#include <Arduino.h>

HangarDoor::HangarDoor(int pin) {
  this->motor = new ServoMotorImpl(pin);
  this->motor->on();
  close();
}

void HangarDoor::open() { this->motor->setPosition(180); }

void HangarDoor::close() { this->motor->setPosition(0); }