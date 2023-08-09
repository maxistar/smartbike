// Copyright 2020 Max Starikov
#ifndef FIRMWARE_SMARTBIKE_OTA_CPP_
#define FIRMWARE_SMARTBIKE_OTA_CPP_


// Copyright 2020 Max Starikov


#include "./Arduino.h"
#include "./Ota.h"

// Set serial for debug console (to the Serial Monitor, default speed 115200)
#define SerialMon Serial

#define DEBUG_PRINT(...) { SerialMon.print(millis()); SerialMon.print(" - "); SerialMon.println(__VA_ARGS__); }
#define DEBUG_FATAL(...) { SerialMon.print(millis()); SerialMon.print(" - FATAL: "); SerialMon.println(__VA_ARGS__); delay(10000); ESP.restart(); }

// URL to download the firmware from

#include "config.h"

#include <Wire.h>
#include <Update.h>
#include <TinyGsmClient.h>


#include "MobileModem.h"

//TinyGsm modem1(SerialAT);

const char server[] = EXTERNAL_URL;     // domain name: example.com, maker.ifttt.com, etc
const int  port = EXTERNAL_URL_PORT;    // server port number

Ota::Ota(MobileModem *modem) {
  mobileModem = modem;
}

void Ota::checkUpdates() {
  SerialMon.println("check new firmware");
  String response = mobileModem->httpGet(server, "/api/firmware/latest", port);
  SerialMon.println("====");
  SerialMon.println(response);
  SerialMon.println("====");
  int ver = response.toInt();
  if (ver == 0) {
    SerialMon.println("Error of checking of new firmware");
    return;
  }
  if (ver != FIRMWARE_VERSION) {
    SerialMon.println("Found new firmware!");
    String updateUrl = OTA_URL + String(ver) + "/sim7000.ino.esp32.bin";
    SerialMon.println(updateUrl);

    DEBUG_PRINT(F("Starting OTA update in 10 seconds..."));
    delay(10000);

    this->startOtaUpdate(updateUrl);
  } else {
    SerialMon.println("No new firmware found");
  }
}

void Ota::startOtaUpdate(const String& ota_url)
{
  String protocol, host, url;
  int port;

  if (!parseURL(ota_url, protocol, host, port, url)) {
    DEBUG_FATAL(F("Cannot parse URL"));
  }

  DEBUG_PRINT(String("Connecting to ") + host + ":" + port);

  Client* client = NULL;
  TinyGsm *modem = mobileModem->getModem();
  //if (protocol == "http") {

    client = new TinyGsmClient(*modem);
    if (!client->connect(host.c_str(), port)) {
      DEBUG_FATAL(F("Client not connected"));
    }
  //} else if (protocol == "https") {
  //  client = new TinyGsmClientSecure(*modem);
  //  if (!client->connect(host.c_str(), port)) {
  //    DEBUG_FATAL(F("Client not connected"));
  //  }
  //} else {
  //  DEBUG_FATAL(String("Unsupported protocol: ") + protocol);
  //}

  DEBUG_PRINT(String("Requesting ") + url);

  client->print(String("GET ") + url + " HTTP/1.0\r\n"
               + "Host: " + host + "\r\n"
               + "Connection: keep-alive\r\n"
               + "\r\n");

  long timeout = millis();
  while (client->connected() && !client->available()) {
    if (millis() - timeout > 10000L) {
      DEBUG_FATAL("Response timeout");
    }
  }

  // Collect headers
  String md5;
  int contentLength = 0;

  while (client->available()) {
    String line = client->readStringUntil('\n');
    line.trim();
    SerialMon.println(line);    // Uncomment this to show response headers
    line.toLowerCase();
    if (line.startsWith("content-length:")) {
      contentLength = line.substring(line.lastIndexOf(':') + 1).toInt();
    } else if (line.startsWith("x-md5:")) {
      md5 = line.substring(line.lastIndexOf(':') + 1);
    } else if (line.length() == 0) {
      break;
    }
  }

  if (contentLength <= 0) {
    DEBUG_FATAL("Content-Length not defined");
  }

  bool canBegin = Update.begin(contentLength);
  if (!canBegin) {
    Update.printError(SerialMon);
    DEBUG_FATAL("OTA begin failed");
  }

  if (md5.length()) {
    DEBUG_PRINT(String("Expected MD5: ") + md5);
    if(!Update.setMD5(md5.c_str())) {
      DEBUG_FATAL("Cannot set MD5");
    }
  }

  DEBUG_PRINT("Flashing...");

  // The next loop does approx. the same thing as Update.writeStream(http) or Update.write(http)

  int written = 0;
  int progress = 0;
  uint8_t buff[256];
  while (client->connected() && written < contentLength) {
    timeout = millis();
    while (client->connected() && !client->available()) {
      delay(1);
      if (millis() - timeout > 10000L) {
        DEBUG_FATAL("Timeout");
      }
    }

    int len = client->read(buff, sizeof(buff));
    if (len <= 0) {
      SerialMon.println("len <= 0 could it be a problem?");
      continue;
    }

    Update.write(buff, len);
    written += len;
    delay(10);

    int newProgress = (written*100)/contentLength;
    if (newProgress - progress >= 5 || newProgress == 100) {
      progress = newProgress;
      SerialMon.print(String("\r ") + progress + "%");
    }
  }
  SerialMon.println();

  if (written != contentLength) {
    Update.printError(SerialMon);
    DEBUG_PRINT(String("Write failed. Written ") + written + " / " + contentLength + " bytes");
  }

  if (!Update.end()) {
    Update.printError(SerialMon);
    DEBUG_FATAL(F("Update not ended"));
  }

  if (!Update.isFinished()) {
    DEBUG_FATAL(F("Update not finished"));
  }

  DEBUG_PRINT("=== Update successfully completed. Rebooting.");
  ESP.restart();
}




void Ota::printDeviceInfo()
{
  Serial.println();
  Serial.println("--------------------------");
  Serial.println(String("Build:    ") +  __DATE__ " " __TIME__);
#if defined(ESP8266)
  Serial.println(String("Flash:    ") + ESP.getFlashChipRealSize() / 1024 + "K");
  Serial.println(String("ESP core: ") + ESP.getCoreVersion());
  Serial.println(String("FW info:  ") + ESP.getSketchSize() + "/" + ESP.getFreeSketchSpace() + ", " + ESP.getSketchMD5());
#elif defined(ESP32)
  Serial.println(String("Flash:    ") + ESP.getFlashChipSize() / 1024 + "K");
  Serial.println(String("ESP sdk:  ") + ESP.getSdkVersion());
  Serial.println(String("Chip rev: ") + ESP.getChipRevision());
#endif
  Serial.println(String("Free mem: ") + ESP.getFreeHeap());
  Serial.println("--------------------------");
}

bool Ota::parseURL(String url, String& protocol, String& host, int& port, String& uri)
{
  int index = url.indexOf(':');
  if(index < 0) {
    return false;
  }

  protocol = url.substring(0, index);
  url.remove(0, (index + 3)); // remove protocol part

  index = url.indexOf('/');
  String server = url.substring(0, index);
  url.remove(0, index);       // remove server part

  index = server.indexOf(':');
  if(index >= 0) {
    host = server.substring(0, index);          // hostname
    port = server.substring(index + 1).toInt(); // port
  } else {
    host = server;
    if (protocol == "http") {
      port = 80;
    } else if (protocol == "https") {
      port = 443;
    }
  }

  if (url.length()) {
    uri = url;
  } else {
    uri = "/";
  }
  return true;
}


#endif  // FIRMWARE_SMARTBIKE_OTA_CPP_
