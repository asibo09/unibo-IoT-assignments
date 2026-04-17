#include "TempTask.h"
#include "../config.h"
#include <Arduino.h>

TempTask::TempTask() {
  this->tempSensor = new TempSensor(PIN_TEMP_SENSOR);
  this->lastTemp = 0;
}

void TempTask::init(int period) { Task::init(period); }

void TempTask::tick() {
  float temp = this->tempSensor->getTemperature();
  if (temp != this->lastTemp) {
    Serial.println("Temperature: " + String(temp) + " C");
    this->lastTemp = temp;
  }
}