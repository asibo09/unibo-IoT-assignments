#ifndef __TEMPSENSOR_H__
#define __TEMPSENSOR_H__

class TempSensor {
  int pin;

public:
  TempSensor(int pin);
  float getTemperature();
};

#endif