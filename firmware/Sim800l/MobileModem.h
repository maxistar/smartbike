#ifndef MOBILE_MODEM_H
#define MOBILE_MODEM_H

#include "Arduino.h"


class MobileModem {
  public:
  void setup();
  void httpPost(String httpRequestData,const char* server,const char* resource, int port);
  void sleepMode();
};

#endif
