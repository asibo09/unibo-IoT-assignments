#include "DoorTask.h"
#include "../config.h"
#include <Arduino.h>

DoorTask::DoorTask() {
  this->door = new HangarDoor(PIN_SERVO);
  this->isOpen = false;
}

void DoorTask::init(int period) { Task::init(period); }

void DoorTask::tick() {
  if (isOpen) {
    door->close();
    Serial.println("Motore HD: Porta Hangar CHIUSA");
    isOpen = false;
  } else {
    door->open();
    Serial.println("Motore HD: Porta Hangar APERTA");
    isOpen = true;
  }
}