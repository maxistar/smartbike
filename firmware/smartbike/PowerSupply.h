#ifndef FIRMWARE_SMARTBIKE_POWERSUPPLY_H_
#define FIRMWARE_SMARTBIKE_POWERSUPPLY_H_

// Copyright 2020 Max Starikov

#include "./Arduino.h"

class PowerSupply {
  public:
  int getVoltage();
  bool setPowerBoostKeepOn(int enable);
  void setup();
  bool getPowerSource();
  bool isBatteryFull();
  uint8_t getBatteryPercentage();
};


#endif  // FIRMWARE_SMARTBIKE_POWERSUPPLY_H_
