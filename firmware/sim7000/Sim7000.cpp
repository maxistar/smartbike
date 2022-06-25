#ifndef FIRMWARE_SMARTBIKE_SMARTBIKE_CPP_
#define FIRMWARE_SMARTBIKE_SMARTBIKE_CPP_

#include "config.h"
#include "Sim7000.h"
#include "Ota.h"



#define TINY_GSM_MODEM_SIM7000
#define TINY_GSM_RX_BUFFER 1024 // Set RX buffer to 1Kb
#define SerialAT Serial1
#define SerialMon Serial
#include <TinyGsmClient.h>
#include <SPI.h>
#include <Ticker.h>

#include "MobileModem.h"

/*
   Tests enabled
*/
#define TINY_GSM_TEST_GPRS    true
#define TINY_GSM_TEST_GPS     true
#define TINY_GSM_POWERDOWN    true


MobileModem mobileModem = MobileModem();
Ota ota = Ota(&mobileModem);

// Server details
// The server variable can be just a domain name or it can have a subdomain. It depends on the service you are using
const char server[] = EXTERNAL_URL;     // domain name: example.com, maker.ifttt.com, etc
const char resource[] = "/api/status/"; // resource path, for example: /post-data.php
const int  port = EXTERNAL_URL_PORT;    // server port number





// TinyGsmClient client(modem);

#define uS_TO_S_FACTOR 1000000ULL  // Conversion factor for micro seconds to seconds
#define TIME_TO_SLEEP  60 * 60          // 1 hour Time ESP32 will go to sleep (in seconds)

#define UART_BAUD   9600
#define PIN_DTR     25
#define PIN_TX      27
#define PIN_RX      26
#define PWR_PIN     4

#define LED_PIN     12

// int counter, lastIndex, numberOfPieces = 24;
// String pieces[24], input;

float batteryVoltage = 0;
float solarVoltage = 0;
float longitude = 0;
float latitude = 0;

#define INPUT_BUFFER_SIZE    1024

String getBatteryDebug() {
    char b[1000];
  
    sprintf(
      b, 
      "{\"latitude\": \"%f\",\n\"longiture\": \"%f\",\n\"battery\": \"%f\",\n\"solarBattery\": \"%f\"}",  
      latitude, longitude, batteryVoltage, solarVoltage
    );

    return String(b);
}

void sendState() {
    // Prepare your HTTP POST request data (Temperature in Celsius degrees)
    String httpRequestData = String("{")
                         + "\"deviceID\": 1,"
                         + "\"version\": " + String(1) + ","
                         + "\"powerSource\":\"" + String("BATTERY") + "\","
                         + "\"batteryStatus\":\"" + String("CHARGING") + "\","
                         + "\"battery\":\"" + String(-1) + "\","
                         + "\"batteryInfo\":" + getBatteryDebug() + "" 
                         + "}";

    mobileModem.httpPost(httpRequestData, server, resource, port);
}

void checkNewFirmware() {
  SerialMon.println("\nWait...");

  delay(10000);
  ota.checkUpdates();
}

/**
 * Setup
 */
void Sim7000::setup()
{
    // Set console baud rate
    SerialMon.begin(9600);
    delay(10);

    // Set LED OFF
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, HIGH);

    pinMode(PWR_PIN, OUTPUT);
    digitalWrite(PWR_PIN, HIGH);
    delay(300);
    digitalWrite(PWR_PIN, LOW);

    SerialMon.println("\nWait...");

    delay(10000);

    SerialAT.begin(UART_BAUD, SERIAL_8N1, PIN_RX, PIN_TX);


    // pinMode(35, ANALOG_INPUT);
    // pinMode(36, ANALOG_INPUT);

    mobileModem.setup();
    SerialMon.println("\nInit Completed...");
  
    SerialMon.println("\nconnect...");
    mobileModem.connectNetwork();
    
    checkNewFirmware(); 
}

/**
 * Loop
 */
void Sim7000::loop()
{
  SerialMon.println("\nStart Loop...");
  TinyGsm *modem = mobileModem.getModem();


  solarVoltage = analogRead(36);
  batteryVoltage = analogRead(35);

  SerialMon.println("\n---Starting GPS TEST---\n");
  // Set SIM7000G GPIO4 HIGH ,turn on GPS power
  // CMD:AT+SGPIO=0,4,1,1
  // Only in version 20200415 is there a function to control GPS power
  modem->sendAT("+SGPIO=0,4,1,1");

  modem->enableGPS();
  while (1) {
    if (modem->getGPS(&latitude, &longitude)) {
      SerialMon.printf("lat:%f lon:%f\n", latitude, longitude);
      break;
    } else {
      SerialMon.print("getGPS ");
      SerialMon.println(millis());
    }
    delay(2000);
  }
  modem->disableGPS();

  // Set SIM7000G GPIO4 LOW ,turn off GPS power
  // CMD:AT+SGPIO=0,4,1,0
  // Only in version 20200415 is there a function to control GPS power
  modem->sendAT("+SGPIO=0,4,1,0");
  SerialMon.println("\n---End of GPRS TEST---\n");

  sendState();

  modem->gprsDisconnect();
  if (!modem->isGprsConnected()) {
    SerialMon.println("GPRS disconnected");
  } else {
    SerialMon.println("GPRS disconnect: Failed.");
  }

  modem->poweroff();
  SerialMon.println("Poweroff.");

  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
  delay(200);
  esp_deep_sleep_start();

  // Do nothing forevermore
  while (true) {
    modem->maintain();
  }
}


#endif  // FIRMWARE_SMARTBIKE_SMARTBIKE_CPP_
