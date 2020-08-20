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

#include "config_common.h"
#include "config_prod.h"

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


/**
 * Setup
 */
void SmartBike::setup()
{
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
  
  // Configure the wake up source as timer wake up
  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
}

/**
 * Loop
 */
void  SmartBike::loop()
{
  sendState();
  
  gpsTracker.loop();

  delay(1000);
  mobileModem.sleepMode();
  delay(1000);

  gpsTracker.forceSleep();

  // Put ESP32 into deep sleep mode (with timer wake up)
  esp_deep_sleep_start();
  //delay(1000);
};

#endif  // FIRMWARE_SMARTBIKE_SMARTBIKE_CPP_
