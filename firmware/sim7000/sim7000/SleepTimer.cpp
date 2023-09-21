#ifndef FIRMWARE_SMARTBIKE_SLEEPTIMER_CPP_
#define FIRMWARE_SMARTBIKE_SLEEPTIMER_CPP_

long long int GetSleepDuration(float voltage, float insulation) {
  if (voltage < 2000) {
      return 6 * 60 * 60;
  }
  if (insulation < 1000) {
    return 4 * 60 * 60;
  }
  if (voltage > 2300) {
        return 60 * 60;
  }
  return 2 * 60 * 60;
}

#endif  // FIRMWARE_SMARTBIKE_SLEEPTIMER_CPP_
