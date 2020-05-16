#ifndef POWER_SUPPLY_H
#define POWER_SUPPLY_H

#include "Arduino.h"

class PowerSupply {
  public:
  int getVoltage();
  bool setPowerBoostKeepOn(int enable);
  void setup();
  bool getPowerSource();
  bool isBatteryFull();
  uint8_t getBatteryPercentage();
};


#endif
