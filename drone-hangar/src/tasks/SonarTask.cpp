#include "SonarTask.h"
#include "../config.h"
#include <Arduino.h>

SonarTask::SonarTask() {
  this->sonar = new Sonar(PIN_SONAR_ECHO, PIN_SONAR_TRIG, 20000);
}

void SonarTask::init(int period) { Task::init(period); }

void SonarTask::tick() {
  float distance = sonar->getDistance();

  Serial.print("Distanza Drone (DDD): ");
  Serial.print(distance);
  Serial.println(" m");
}