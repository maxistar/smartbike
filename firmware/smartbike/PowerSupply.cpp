#ifndef POWER_SUPPLY_CPP
#define POWER_SUPPLY_CPP

// Copyright 2020 Max Starikov


#include "./PowerSupply.h"
#include "./Arduino.h"

#include "./IP5306.h"


// I2C for SIM800 (to keep it running when powered from battery)


void PowerSupply::setup() {
  // Start I2C communication
  IP5306_setup();
}

int PowerSupply::getVoltage() {
  return 0;
};

/**
 * 
 */
bool PowerSupply::getPowerSource() {
  return IP5306_GetPowerSource();
}

bool PowerSupply::isBatteryFull() {
  return IP5306_GetBatteryFull();
}

uint8_t PowerSupply::getBatteryPercentage() {
  return IP5306_LEDS2PCT(IP5306_GetLevelLeds());
}

bool PowerSupply::setPowerBoostKeepOn(int en) {
  return IP5306_setPowerBoostKeepOn(en);
}


#endif
