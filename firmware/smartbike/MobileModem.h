#ifndef FIRMWARE_SMARTBIKE_MOBILEMODEM_H_
#define FIRMWARE_SMARTBIKE_MOBILEMODEM_H_

// Copyright 2020 Max Starikov

#include "./Arduino.h"


class MobileModem {
  public:
  void setup();
  void httpPost(String httpRequestData, const char* server, const char* resource, int port);
  void sleepMode();
};

#endif  // FIRMWARE_SMARTBIKE_MOBILEMODEM_H_
