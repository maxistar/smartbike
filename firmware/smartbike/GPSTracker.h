#ifndef FIRMWARE_SMARTBIKE_GPSTRACKER_H_
#define FIRMWARE_SMARTBIKE_GPSTRACKER_H_

// Copyright 2020 Max Starikov

#include "./Arduino.h"



class GPSTracker {
  public:
  GPSTracker();
  void setup();
  void loop();
  void forceSleep();
};

#endif  // FIRMWARE_SMARTBIKE_GPSTRACKER_H_
