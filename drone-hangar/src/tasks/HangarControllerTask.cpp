#include "HangarControllerTask.h"
#include "../config.h"
#include <Arduino.h>

HangarControllerTask::HangarControllerTask(BlinkTask *blinkerTask) {
  this->blinker = blinkerTask;

  this->door = new HangarDoor(PIN_SERVO);
  this->sonar = new Sonar(PIN_SONAR_ECHO, PIN_SONAR_TRIG, 20000);
  this->pir = new Pir(PIN_PIR);
  this->l1 = new Led(PIN_LED_L1);

  this->lcd = new LiquidCrystal_I2C(0x27, 16, 2);
  this->lcd->init();
  this->lcd->backlight();

  this->state = DRONE_INSIDE;
  this->takeOffCmd = false;
  this->landingCmd = false;
}

void HangarControllerTask::init(int period) {
  Task::init(period);
  // setup iniziale: L1 acceso, porta chiusa, LCD DRONE INSIDE
  updateLCD("DRONE INSIDE");
  l1->switchOn();
  blinker->setBlinking(false);
  door->close();
}

void HangarControllerTask::updateLCD(const char *message) {
  lcd->clear();
  lcd->setCursor(0, 0);
  lcd->print(message);
}

void HangarControllerTask::commandTakeOff() { takeOffCmd = true; }
void HangarControllerTask::commandLanding() { landingCmd = true; }

void HangarControllerTask::tick() {
  pir->sync();
  float dist = sonar->getDistance();

  switch (state) {
  case DRONE_INSIDE:
    // stato DRONE_INSIDE -> porta chiusa, L1 acceso, scritta su LCD
    if (takeOffCmd) {
      takeOffCmd = false;
      state = TAKE_OFF;
      door->open();
      updateLCD("TAKE OFF");
      blinker->setBlinking(true);
      ticksCount = 0;
    }
    break;

  case TAKE_OFF:
    // stato TAKE_OFF -> porta aperta, L2 lampeggia, scritta su LCD
    if (dist > DIST_D1) {
      ticksCount++; // conteggio secondi per cambiare stato se dist > D1 per
                    //  TIME_T1
      if (ticksCount >= TIME_T1) {
        state = DRONE_OUT; // cambio stato in DRONE_OUT
        door->close();
        updateLCD("DRONE OUT");
        l1->switchOff();
        blinker->setBlinking(false);
      }
    } else {
      ticksCount = 0; // reset timer se dist < D1
    }
    break;

  case DRONE_OUT:
    // stato DRONE_OUT -> porta chiusa, L1 spenta, scritta su LCD
    if (landingCmd) { // comando atterraggio ricevuto
      landingCmd = false;
      state = WAIT_FOR_DRONE; // cambio stato in WAIT_FOR_DRONE
      updateLCD("WAITING DRONE");
    }
    break;

  case WAIT_FOR_DRONE:
    // stato WAIT_FOR_DRONE -> porta chiusa, L1 spenta, scritta su LCD
    if (pir->isDetected()) { // quando PIR rileva il drone
      state = LANDING;       // cambio stato in LANDING
      door->open();
      updateLCD("LANDING");
      blinker->setBlinking(true);
      ticksCount = 0;
    }
    break;

  case LANDING:
    // stato LANDING -> porta aperta, L2 lampeggia, scritta su LCD
    if (dist < DIST_D2 && dist > 0.0) {
      ticksCount++; // conteggio secondi per cambiare stato se dist > D2 per
                    // TIME_T2
      if (ticksCount >= TIME_T2) {
        state = DRONE_INSIDE; // cambio stato in DRONE_INSIDE
        door->close();
        updateLCD("DRONE INSIDE");
        l1->switchOn();
        blinker->setBlinking(false);
      }
    } else {
      ticksCount = 0;
    }
    break;
  }
}