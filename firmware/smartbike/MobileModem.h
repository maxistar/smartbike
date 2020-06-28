#ifndef FIRMWARE_SMARTBIKE_MOBILEMODEM_H_
#define FIRMWARE_SMARTBIKE_MOBILEMODEM_H_

// Copyright 2020 Max Starikov

#include "./Arduino.h"

// Configure TinyGSM library
#define TINY_GSM_MODEM_SIM800      // Modem is SIM800
#define TINY_GSM_RX_BUFFER   1024  // Set RX buffer to 1Kbs
#include <TinyGsmClient.h>


class MobileModem {
  public:
  MobileModem();
  ~MobileModem();
  void setup();
  void httpPost(String httpRequestData, const char* server, const char* resource, int port);
  String httpGet(const char* server, const char* resource, int port);
  void sleepMode();
  TinyGsm *getModem();

  private:
  TinyGsm *modem;
  TinyGsmClient *client;
};

#endif  // FIRMWARE_SMARTBIKE_MOBILEMODEM_H_
