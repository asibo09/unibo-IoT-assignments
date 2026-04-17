#include "TempControllerTask.h"
#include "../config.h"
#include "../kernel/MsgService.h"
#include "HangarControllerTask.h"
#include "TempControllerTAsk.h"
#include "devices/Led.h"
#include "devices/TempSensor.h"
#include "kernel/Task.h"
#include <Arduino.h>

TempControllerTask::TempControllerTask(HangarControllerTask *hangar) {
  this->hangarTask = hangar;
  this->tempSensor = new TempSensor(PIN_TEMP_SENSOR);
  this->btnReset = new ButtonImpl(PIN_BUTTON_RESET);
  this->l3 = new Led(PIN_LED_L3);
  this->state = NORMAL;
}

void TempControllerTask::init(int period) {
  Task::init(period);
  l3->switchOff();
}

void TempControllerTask::tick() {
  float temp = tempSensor->getTemperature();

  switch (state) {
  case NORMAL:
    // se temp supera TEMP2 va in CHECK_ALARM, se supera TEMP1 va in CHECK_PRE
    if (temp >= TEMP_2) {
      state = CHECK_ALARM;
      ticksCount = 0;
    } else if (temp >= TEMP_1) {
      state = CHECK_PRE;
      ticksCount = 0;
    }
    break;
  case CHECK_PRE:
    if (temp < TEMP_1) {
      // torna stato NORMAL
      state = NORMAL;
    } else if (temp >= TEMP_2) {
      // va in stato CHECK_ALARM
      state = CHECK_ALARM;
      ticksCount = 0;
    } else {
      ticksCount++;
      if (ticksCount >= TIME_T3) {
        // in stato di PRE_ALARM, comunica ad hangar
        state = PRE_ALARM;
        hangarTask->setPreAlarm(true);
        MsgService.sendMsg("LOG: TEMP FSM -> PRE-ALLARME ATTIVO");
      }
    }
    break;
  case PRE_ALARM:
    if (temp >= TEMP_2) {
      // manda in segnale di CHECK_ALARM
      state = CHECK_ALARM;
      ticksCount = 0;
    } else if (temp < TEMP_1) {
      // torna stato NORMAL comunica ad hangar di rimuovere stato di pre-alarm
      state = NORMAL;
      hangarTask->setPreAlarm(false);
      MsgService.sendMsg("LOG: TEMP FSM -> PRE-ALLARME RIENTRATO");
    }
    break;
  case CHECK_ALARM:
    if (temp < TEMP_2) {
      state = NORMAL;
    } else {
      // se si super T4 va in stato di ALARM, accende L3 rosso e blocca hangar
      ticksCount++;
      if (ticksCount >= TIME_T4) {
        state = ALARM;
        l3->switchOn();
        hangarTask->triggerAlarm();
        MsgService.sendMsg("LOG: TEMP FSM -> ALLARME!!! SISTEMA BLOCCATO");
      }
    }
    break;
  case ALARM:
    // sblocca e torna NORMAL solo se: si preme button reset e temp tornata
    // inferiore a TEMP1
    if (btnReset->isPressed()) {
      if (temp < TEMP_1) {
        state = NORMAL;
        l3->switchOff();
        hangarTask->resetAlarm();
        MsgService.sendMsg("LOG: TEMP FSM -> Sistema resettato");
      } else {
        MsgService.sendMsg("LOG: TEMP FSM -> Impossibile resettare. "
                           "Temperatura ancora anomala");
      }
    }
    break;
  }
}