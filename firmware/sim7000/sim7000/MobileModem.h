#ifndef FIRMWARE_SMARTBIKE_MOBILEMODEM_H_
#define FIRMWARE_SMARTBIKE_MOBILEMODEM_H_

// Copyright 2020 Max Starikov

#include "./Arduino.h"
#include "config.h"
#include <TinyGsmClient.h>


class MobileModem {
  public:
  MobileModem();
  ~MobileModem();
  void setup();
  bool connectNetwork();
  void httpPost(String httpRequestData, const char* server, const char* resource, int port);
  String httpGet(const char* server, const char* resource, int port);
  void sleepMode();
  TinyGsm *getModem();

  private:
  TinyGsm *modem;
  TinyGsmClient *client;
};

#endif  // FIRMWARE_SMARTBIKE_MOBILEMODEM_H_
