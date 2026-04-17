#include "PirTask.h"
#include "../config.h"
#include <Arduino.h>

PirTask::PirTask() {
  this->pir = new Pir(PIN_PIR);
  this->lastState = false;
}

void PirTask::init(int period) { Task::init(period); }

void PirTask::tick() {
  pir->sync();
  bool isDetected = pir->isDetected();
  if (isDetected != lastState) {
    if (isDetected) {
      Serial.println(
          "Sensore DPD: Drone RILEVATO sopra l'hangar! [PRESENZA ON]");
    } else {
      Serial.println("Sensore DPD: Drone NON presente. [PRESENZA OFF]");
    }
    lastState = isDetected;
  }
}