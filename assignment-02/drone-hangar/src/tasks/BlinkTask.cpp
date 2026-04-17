#include "BlinkTask.h"

BlinkTask::BlinkTask(int pin) {
  this->led = new Led(pin);
  this->isBlinking = false;
  this->state = 0; // 0 = Spento, 1 = Acceso
}

// Inizializzazione task
void BlinkTask::init(int period) {
  Task::init(period);
  this->led->switchOff();
}

// comanda al task di iniziare/smettere di lampeggiare
void BlinkTask::setBlinking(bool blink) {
  this->isBlinking = blink;
  if (!blink) {
    this->led->switchOff();
    this->state = 0;
  }
}

// funzione che lo Scheduler chiama automaticamente ogni period
void BlinkTask::tick() {
  if (isBlinking) {
    if (state == 0) {
      led->switchOn();
      state = 1;
    } else {
      led->switchOff();
      state = 0;
    }
  }
}