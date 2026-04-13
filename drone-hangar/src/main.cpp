#include "config.h"
#include "kernel/MsgService.h"
#include "kernel/Scheduler.h"
#include "tasks/BlinkTask.h"
#include "tasks/HangarControllerTask.h"
#include "tasks/TempControllerTask.h"
#include <Arduino.h>

Scheduler sched;
BlinkTask *blinkTask;
HangarControllerTask *hangarTask;
TempControllerTask *tempControllerTask;

void setup() {
  MsgService.init();
  sched.init(50);

  blinkTask = new BlinkTask(PIN_LED_L2);
  blinkTask->init(500);
  sched.addTask(blinkTask);

  hangarTask = new HangarControllerTask(blinkTask);
  hangarTask->init(100);
  sched.addTask(hangarTask);

  tempControllerTask = new TempControllerTask(hangarTask);
  tempControllerTask->init(100);
  sched.addTask(tempControllerTask);
}

void loop() {
  sched.schedule();

  if (MsgService.isMsgAvailable()) {
    Msg *msg = MsgService.receiveMsg();
    String cmd = msg->getContent();

    if (cmd == "TAKE_OFF") {
      hangarTask->commandTakeOff();
    } else if (cmd == "LANDING") {
      hangarTask->commandLanding();
    }

    delete msg;
  }
}