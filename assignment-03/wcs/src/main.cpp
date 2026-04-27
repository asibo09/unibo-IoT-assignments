#include "config.h"
#include "kernel/MsgService.h"
#include "kernel/Scheduler.h"
#include "tasks/WcsControllerTask.h"
#include <Arduino.h>


Scheduler sched;
WcsControllerTask *wcsTask;

void setup() {
  MsgService.init();
  sched.init(50);

  wcsTask = new WcsControllerTask();
  wcsTask->init(100);
  sched.addTask(wcsTask);
}

void loop() { sched.schedule(); }