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
  return 
    String("{") +
    printf("KeyOff: %s,\n", IP5306_GetKeyOffEnabled() ? "Enabled" : "Disabled") +
    printf("BoostOutput: %s,\n", IP5306_GetBoostOutputEnabled() ? "Enabled" : "Disabled") +
    printf("PowerOnLoad: %s,\n", IP5306_GetPowerOnLoadEnabled() ? "Enabled" : "Disabled") +
    printf("Charger: %s,\n", IP5306_GetChargerEnabled() ? "Enabled" : "Disabled") +
    printf("Boost: %s,\n", IP5306_GetBoostEnabled() ? "Enabled" : "Disabled") +
    printf("LowBatShutdown: %s,\n", IP5306_GetLowBatShutdownEnable() ? "Enabled" : "Disabled") +
    printf("ShortPressBoostSwitch: %s,\n", IP5306_GetShortPressBoostSwitchEnable() ? "Enabled" : "Disabled") +
    printf("FlashlightClicks: %s,\n", IP5306_GetFlashlightClicks() ? "Long Press" : "Double Press") +
    printf("BoostOffClicks: %s,\n", IP5306_GetBoostOffClicks() ? "Double Press" : "Long Press") +
    printf("BoostAfterVin: %s,\n", IP5306_GetBoostAfterVin() ? "Open" : "Not Open") +
    printf("LongPressTime: %s,\n", IP5306_GetLongPressTime() ? "3s" : "2s") +
    printf("ChargeUnderVoltageLoop: %.2fV,\n", 4.45 + (IP5306_GetChargeUnderVoltageLoop() * 0.05)) +
    printf("ChargeCCLoop: %s,\n", IP5306_GetChargeCCLoop() ? "Vin" : "Bat") +
    printf("VinCurrent: %dmA,\n", (IP5306_GetVinCurrent() * 100) + 50) +
    printf("VoltagePressure: %dmV,\n", IP5306_GetVoltagePressure() * 14) +
    printf("ChargingFullStopVoltage: %u,\n", IP5306_GetChargingFullStopVoltage()) +
    printf("LightLoadShutdownTime: %u,\n", IP5306_GetLightLoadShutdownTime()) +
    printf("EndChargeCurrentDetection: %u,\n", IP5306_GetEndChargeCurrentDetection()) +
    printf("ChargeCutoffVoltage: %u\n", IP5306_GetChargeCutoffVoltage()) +
    "}";
}


#endif
