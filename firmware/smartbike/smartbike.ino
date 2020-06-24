/*
 * sends data to https://requestbin.com/r/enx5hjar0vnp/1ZoKHwU0sx3NiaVObRVsStA7eFE  
 */


#include "PowerSupply.h"
#include "MobileModem.h"
#include "Ota.h"
#define SerialMon Serial

// Server details
// The server variable can be just a domain name or it can have a subdomain. It depends on the service you are using
const char server[] = "enx5hjar0vnp.x.pipedream.net"; // domain name: example.com, maker.ifttt.com, etc
const char resource[] = "/";         // resource path, for example: /post-data.php
const int  port = 80;                             // server port number



PowerSupply powerSupply = PowerSupply(); 
MobileModem mobileModem = MobileModem();
Ota ota = Ota();

/* Conversion factor for micro seconds to seconds */
#define uS_TO_S_FACTOR 1000000     

/* Time ESP32 will go to sleep (in seconds) 600 seconds = 10 minutes */
#define TIME_TO_SLEEP  600        




void setup() {
  // Set serial monitor debugging window baud rate to 115200
  SerialMon.begin(115200);

  powerSupply.setup();

  // Keep power when running from battery
  bool isOk = powerSupply.setPowerBoostKeepOn(1);
  
  SerialMon.println(String("IP5306 KeepOn ") + (isOk ? "OK" : "FAIL"));

  mobileModem.setup();

  
  // Configure the wake up source as timer wake up  
  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
}

void sendState() {
  bool usb = powerSupply.getPowerSource();
  bool full = powerSupply.isBatteryFull();
  
  // Prepare your HTTP POST request data (Temperature in Celsius degrees)
  String httpRequestData = "powerSource=" + String(usb ? "USB" : "BATTERY") 
                         + "&batteryFull=" + String(full ? "CHARGED" : (usb ? "CHARGING" : "DISCHARGING")) 
                         + "&batteryPercentage=" + String(powerSupply.getBatteryPercentage());

  mobileModem.httpPost(httpRequestData, server, resource, port);

  mobileModem.sleepMode();
}

void checkNewFirmware() {
   
}

void loop() {
  checkNewFirmware();
  sendState();
  
  // Put ESP32 into deep sleep mode (with timer wake up)
  esp_deep_sleep_start();

}
