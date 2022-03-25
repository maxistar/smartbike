#ifndef FIRMWARE_SMARTBIKE_SMARTBIKE_CPP_
#define FIRMWARE_SMARTBIKE_SMARTBIKE_CPP_

// Copyright 2020 Max Starikov

#include <WiFi.h>

#include "./Arduino.h"
#include "SmartBike.h"
#include "PowerSupply.h"
#include "MobileModem.h"
#include "GPSTracker.h"
#include "Ota.h"

#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"

#include "config_common.h"
#include "config_prod.h"

#define MODEM_POWER_ON 23

#define SerialMon Serial

// Server details
// The server variable can be just a domain name or it can have a subdomain. It depends on the service you are using
const char server[] = EXTERNAL_URL;     // domain name: example.com, maker.ifttt.com, etc
const char resource[] = "/api/status/"; // resource path, for example: /post-data.php
const int  port = EXTERNAL_URL_PORT;    // server port number


PowerSupply powerSupply = PowerSupply();
MobileModem mobileModem = MobileModem();
Ota ota = Ota(&mobileModem);
GPSTracker gpsTracker = GPSTracker(); 

/* Conversion factor for micro seconds to seconds */
#define uS_TO_S_FACTOR 1000000


void sendState() {
  bool usb = powerSupply.getPowerSource();
  bool full = powerSupply.isBatteryFull();

  // Prepare your HTTP POST request data (Temperature in Celsius degrees)
  String httpRequestData = String("{")
                         + "\"deviceID\": 1,"
                         + "\"version\": " + String(FIRMWARE_VERSION) + ","
                         + "\"powerSource\":\"" + String(usb ? "USB" : "BATTERY") + "\","
                         + "\"batteryStatus\":\"" + String(full ? "CHARGED" : (usb ? "CHARGING" : "DISCHARGING")) + "\","
                         + "\"battery\":\"" + String(powerSupply.getBatteryPercentage()) + "\","
                         + "\"batteryInfo\":" + powerSupply.getBatteryDebug() + "" 
                         + "}";

  mobileModem.httpPost(httpRequestData, server, resource, port);

  
}


void checkNewFirmware() {
  ota.checkUpdates();
}

void gotoSleep() {
  // Configure the wake up source as timer wake up
  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
  // Put ESP32 into deep sleep mode (with timer wake up)

  digitalWrite(MODEM_POWER_ON, LOW);
  
  esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_PERIPH, ESP_PD_OPTION_OFF);
  esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_FAST_MEM, ESP_PD_OPTION_OFF);
  esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_SLOW_MEM, ESP_PD_OPTION_OFF);
  esp_deep_sleep_start();
}


/**
 * Setup
 */
void SmartBike::setup()
{
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); //disable brownout detector
  
  digitalWrite(MODEM_POWER_ON, HIGH);
  
  // Set serial monitor debugging window baud rate to 115200
  SerialMon.begin(115200);

  powerSupply.setup();

  WiFi.mode(WIFI_MODE_NULL);

  // Keep power when running from battery
  bool isOk = powerSupply.setPowerBoostKeepOn(1);
  SerialMon.println(String("IP5306 KeepOn ") + (isOk ? "OK" : "FAIL"));

  gpsTracker.setup();
  mobileModem.setup();
  

  checkNewFirmware(); 
}

/**
 * Loop
 */
void SmartBike::loop()
{
  sendState();
  
  gpsTracker.loop();

  delay(1000);
  mobileModem.sleepMode();
  delay(1000);

  gpsTracker.forceSleep();

  delay(1000);

  gotoSleep();
};

#endif  // FIRMWARE_SMARTBIKE_SMARTBIKE_CPP_
