#include "WcsControllerTask.h"
#include "../config.h"
#include "../kernel/MsgService.h"
#include <Arduino.h>

WcsControllerTask::WcsControllerTask() {
  this->btnMode = new ButtonImpl(PIN_BUTTON);
  this->pot = new Potentiometer(PIN_POT);
  this->servo = new ServoMotorImpl(PIN_SERVO);
  this->lcd = new LiquidCrystal_I2C(0x27, 16, 2);

  // di default automatic e valvola 0%
  this->state = AUTOMATIC;
  this->currentValveLevel = 0;
  this->lastBtnState = false;
}

void WcsControllerTask::init(int period) {
  Task::init(period);
  lcd->init();
  lcd->backlight();

  servo->on();
  setValve(0);
  updateLCD();
}

// calcola gradi per il servo
void WcsControllerTask::setValve(int percentage) {
  if (percentage < 0)
    percentage = 0;
  if (percentage > 100)
    percentage = 100;

  currentValveLevel = percentage;
  // 0% = 0 gradi, 100% = 90 gradi
  int angle = map(percentage, 0, 100, VALVE_CLOSED_POS, VALVE_OPEN_POS);
  servo->setPosition(angle);
}

void WcsControllerTask::updateLCD() {
  lcd->clear();
  lcd->setCursor(0, 0);
  if (state == AUTOMATIC) {
    lcd->print("MODE: AUTOMATIC");
  } else if (state == MANUAL) {
    lcd->print("MODE: MANUAL");
  } else if (state == UNCONNECTED) {
    lcd->print("MODE: UNCONNECTED");
  }

  lcd->setCursor(0, 1);
  lcd->print("VALVE: ");
  lcd->print(currentValveLevel);
  lcd->print("%");
}

void WcsControllerTask::tick() {
  this->pot->sync();
  // gestione messaggi seriali dal CUS
  if (MsgService.isMsgAvailable()) {
    Msg *msg = MsgService.receiveMsg();
    String cmd = msg->getContent();
    delete msg;
    cmd.trim();

    if (cmd == "ERR") {
      state = UNCONNECTED;
      updateLCD();
    } else if (cmd == "RESTORE") {
      state = AUTOMATIC;
      updateLCD();
    } else if (cmd == "FORCE_MANUAL") {
      state = MANUAL;
      updateLCD();
    } else if (cmd == "FORCE_AUTO") {
      state = AUTOMATIC;
      updateLCD();
    } else if (cmd.startsWith("VALVE:")) {
      // estrazione numero dopo "VALVE:"
      int level = cmd.substring(6).toInt();
      setValve(level);
      updateLCD();
    }
  }

  // lettura del bottone
  bool currentBtnState = btnMode->isPressed();
  bool btnClicked = (currentBtnState && !lastBtnState);
  lastBtnState = currentBtnState;

  // macchina a stati
  switch (state) {
  case AUTOMATIC:
    // se in automatico premo il bottone, passo in MANUAL
    if (btnClicked) {
      state = MANUAL;
      MsgService.sendMsg("STATE:MANUAL");
      updateLCD();
    }
    break;

  case MANUAL:
    // se in manuale premo il bottone, torno in AUTOMATIC
    if (btnClicked) {
      state = AUTOMATIC;
      MsgService.sendMsg("STATE:AUTOMATIC");
      updateLCD();
    } else {
      float potValue = pot->getValue();
      int mappedLevel = (int)(potValue * 100.0);

      static int lastPotLevel = -1;
      // aggiornamento valvola solo se valore cambia di almeno 2; così non
      // combatte con slider di operatore remoto da web
      if (abs(mappedLevel - lastPotLevel) >= 2) {
        lastPotLevel = mappedLevel;
        setValve(mappedLevel);
        updateLCD();
        MsgService.sendMsg("VALVE:" + String(currentValveLevel));
      }
    }
    break;

  case UNCONNECTED:
    // la valvola si blocca
    // aspetta solo il messaggio "RESTORE" dalla seriale
    break;
  }
}