// Copyright 2020 Max Starikov
#ifndef FIRMWARE_SMARTBIKE_OTA_H_
#define FIRMWARE_SMARTBIKE_OTA_H_

#include "./Arduino.h"
#include "MobileModem.h"

class Ota {
  public:
  Ota(MobileModem *modem);
  void checkUpdates();

  private:
  MobileModem *mobileModem;
  bool parseURL(String url, String& protocol, String& host, int& port, String& uri);
  void printDeviceInfo();
  void startOtaUpdate(const String& ota_url);
};

#endif  // FIRMWARE_SMARTBIKE_OTA_H_
