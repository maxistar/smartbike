#ifndef FIRMWARE_SMARTBIKE_CONFIG_PROD_H_
#define FIRMWARE_SMARTBIKE_CONFIG_PROD_H_

/**
 * Here located production configs
 */

#define EXTERNAL_URL "bike.x.maxistar.me"
#define EXTERNAL_URL_PORT 80

#define OTA_URL "http://bike.x.maxistar.me/firmware/"

#define TIME_TO_SLEEP  60 * 60          // 10 minutes, time ESP32 will go to sleep (in seconds)

#endif  // FIRMWARE_SMARTBIKE_CONFIG_PROD_H_
