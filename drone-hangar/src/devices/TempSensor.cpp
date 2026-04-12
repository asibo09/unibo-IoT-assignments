#include "TempSensor.h"
#include <Arduino.h>

TempSensor::TempSensor(int pin) { this->pin = pin; }

float TempSensor::getTemperature() {
  int analogValue = analogRead(pin);
  float celsius =
      1 / (log(1 / (1023. / analogValue - 1)) / 3950.0 + 1.0 / 298.15) - 273.15;
  return celsius;
}