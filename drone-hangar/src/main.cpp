#include "config.h"
#include "kernel/Scheduler.h"
#include "tasks/BlinkTask.h"
#include "tasks/HangarControllerTask.h"
#include "tasks/TempTask.h"
#include <Arduino.h>

Scheduler sched;
BlinkTask *blinkTask;
HangarControllerTask *hangarTask;
TempTask *tempTask;

void setup() {
  Serial.begin(9600);
  sched.init(50);

  blinkTask = new BlinkTask(PIN_LED_L2);
  blinkTask->init(500);
  sched.addTask(blinkTask);

  hangarTask = new HangarControllerTask(blinkTask);
  hangarTask->init(100);
  sched.addTask(hangarTask);

  tempTask = new TempTask();
  tempTask->init(500);
  sched.addTask(tempTask);
}

void loop() {
  sched.schedule();

  if (Serial.available()) {
    char cmd = Serial.read();
    if (cmd == 't') {
      Serial.println("Comando inviato: DECOLLO");
      hangarTask->commandTakeOff();
    } else if (cmd == 'l') {
      Serial.println("Comando inviato: ATTERRAGGIO");
      hangarTask->commandLanding();
    }
  }
}