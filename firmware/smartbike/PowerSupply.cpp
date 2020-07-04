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

String PowerSupply::getBatteryDebug() {
    char b[1000];
  
    sprintf(
      b, 
      "{\"KeyOff\": \"%s\",\n\"BoostOutput\": \"%s\",\n\"PowerOnLoad\": \"%s\",\n\"Charger\": \"%s\",\n\"Boost\": \"%s\",\n\"LowBatShutdown\": \"%s\",\n\"ShortPressBoostSwitch\": \"%s\",\n\"FlashlightClicks\": \"%s\",\n\"BoostOffClicks\": \"%s\",\n\"BoostAfterVin\": \"%s\",\n\"LongPressTime\": \"%s\",\n\"ChargeUnderVoltageLoop\": \"%.2fV\",\n\"ChargeCCLoop\": \"%s\",\n\"VinCurrent\": \"%dmA\",\n\"VoltagePressure\": \"%dmV\",\n\"ChargingFullStopVoltage\": %u,\n\"EndChargeCurrentDetection\": %u,\n\"ChargeCutoffVoltage\": %u}",  
      IP5306_GetKeyOffEnabled() ? "Enabled" : "Disabled",
      IP5306_GetBoostOutputEnabled() ? "Enabled" : "Disabled",
      IP5306_GetPowerOnLoadEnabled() ? "Enabled" : "Disabled",
      IP5306_GetChargerEnabled() ? "Enabled" : "Disabled",
      IP5306_GetBoostEnabled() ? "Enabled" : "Disabled",
      IP5306_GetLowBatShutdownEnable() ? "Enabled" : "Disabled",
      IP5306_GetShortPressBoostSwitchEnable() ? "Enabled" : "Disabled",
      IP5306_GetFlashlightClicks() ? "Long Press" : "Double Press",
      IP5306_GetBoostOffClicks() ? "Double Press" : "Long Press",
      IP5306_GetBoostAfterVin() ? "Open" : "Not Open",
      IP5306_GetLongPressTime() ? "3s" : "2s",
      4.45 + (IP5306_GetChargeUnderVoltageLoop() * 0.05),
      IP5306_GetChargeCCLoop() ? "Vin" : "Bat",
      IP5306_GetVinCurrent() * 100 + 50,
      IP5306_GetVoltagePressure() * 14,
      IP5306_GetChargingFullStopVoltage(),
      IP5306_GetLightLoadShutdownTime(),
      IP5306_GetEndChargeCurrentDetection(),
      IP5306_GetChargeCutoffVoltage()
    );

    return String(b);
}


#endif
