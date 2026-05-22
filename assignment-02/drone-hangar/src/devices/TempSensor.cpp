#include "TempSensor.h"
#include <Arduino.h>

TempSensor::TempSensor(int pin) { this->pin = pin; }

float TempSensor::getTemperature() {
  int analogValue = analogRead(pin);
  float voltage = (analogValue / 1024.0) * 5.0;
  float celsius = (voltage - 0.5) * 100.0;

  return celsius;
}