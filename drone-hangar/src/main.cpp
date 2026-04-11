#include "config.h"
#include "kernel/Scheduler.h"
#include "tasks/BlinkTask.h"
#include "tasks/PirTask.h"
#include "tasks/SonarTask.h"
#include <Arduino.h>

Scheduler sched;
BlinkTask *blinkTask;
SonarTask *sonarTask;
PirTask *pirTask;

void setup() {
  Serial.begin(9600);
  sched.init(50); // Tick base dello scheduler ogni 50ms

  // LED
  blinkTask = new BlinkTask(PIN_LED_L2);
  blinkTask->init(500);
  blinkTask->setBlinking(true);
  sched.addTask(blinkTask);

  // setup Sonar
  sonarTask = new SonarTask();
  sonarTask->init(500);
  sched.addTask(sonarTask);

  // setup Pir
  pirTask = new PirTask();
  pirTask->init(200);
  sched.addTask(pirTask);
}

void loop() { sched.schedule(); }