#ifndef MOBILE_MODEM_CPP
#define MOBILE_MODEM_CPP

// Copyright 2020 Max Starikov

#include <TinyGsmClient.h>
#include "./Arduino.h"
#include "./MobileModem.h"

// TTGO T-Call pins
#define MODEM_RST            5
#define MODEM_PWKEY          4
#define MODEM_POWER_ON       23
#define MODEM_TX             27
#define MODEM_RX             26

// Your GPRS credentials (leave empty, if not needed)
const char apn[]      = "chili";  // APN
const char gprsUser[] = "";  // GPRS User
const char gprsPass[] = "";  // GPRS Password

// SIM card PIN (leave empty, if not defined)
const char simPIN[] = "";


// Set serial for debug console (to Serial Monitor, default speed 115200)
#define SerialMon Serial
// Set serial for AT commands (to SIM800 module)
#define SerialAT Serial1

// Configure TinyGSM library
#define TINY_GSM_MODEM_SIM800      // Modem is SIM800
#define TINY_GSM_RX_BUFFER   1024  // Set RX buffer to 1Kb

// Define the serial console for debug prints, if needed
// #define DUMP_AT_COMMANDS


#ifdef DUMP_AT_COMMANDS
  #include <StreamDebugger.h>
  StreamDebugger debugger(SerialAT, SerialMon);
  TinyGsm modem(debugger);
#else
  TinyGsm modem(SerialAT);
#endif


// TinyGSM Client for Internet connection
TinyGsmClient client(modem);

void MobileModem::setup() {
  // Set modem reset, enable, power pins
  pinMode(MODEM_PWKEY, OUTPUT);
  pinMode(MODEM_RST, OUTPUT);
  pinMode(MODEM_POWER_ON, OUTPUT);
  digitalWrite(MODEM_PWKEY, LOW);
  digitalWrite(MODEM_RST, HIGH);
  digitalWrite(MODEM_POWER_ON, HIGH);

  // Set GSM module baud rate and UART pins
  SerialAT.begin(115200, SERIAL_8N1, MODEM_RX, MODEM_TX);

  delay(3000);
  // Restart SIM800 module, it takes quite some time
  // To skip it, call init() instead of restart()
  SerialMon.println("Initializing modem...");
  modem.restart();
  // use modem.init() if you don't need the complete restart

  // Unlock your SIM card with a PIN if needed
  if (strlen(simPIN) && modem.getSimStatus() != 3) {
    modem.simUnlock(simPIN);
  }
}

void MobileModem::httpPost(
  String httpRequestData,
  const char* server,
  const char* resource,
  int port
) {
  SerialMon.print("Connecting to APN: ");
  SerialMon.print(apn);
  if (!modem.gprsConnect(apn, gprsUser, gprsPass)) {
    SerialMon.println(" fail");
  } else {
    SerialMon.println(" OK");

    SerialMon.print("Connecting to ");
    SerialMon.print(server);
    if (!client.connect(server, port)) {
      SerialMon.println(" fail");
    } else {
      SerialMon.println(" OK");

      // Making an HTTP POST request
      SerialMon.println("Performing HTTP POST request...");

      client.print(String("POST ") + resource + " HTTP/1.1\r\n");
      client.print(String("Host: ") + server + "\r\n");
      client.println("Connection: close");
      client.println("Content-Type: application/x-www-form-urlencoded");
      client.print("Content-Length: ");
      client.println(httpRequestData.length());
      client.println();
      client.println(httpRequestData);

      unsigned long timeout = millis();
      while (client.connected() && millis() - timeout < 10000L) {
        // Print available data (HTTP response from server)
        while (client.available()) {
          char c = client.read();
          SerialMon.print(c);
          timeout = millis();
        }
      }
      SerialMon.println();

      // Close client and disconnect
      client.stop();
      SerialMon.println(F("Server disconnected"));
      modem.gprsDisconnect();
      SerialMon.println(F("GPRS disconnected"));
    }
  }
}

void MobileModem::sleepMode() {
    SerialMon.println(F("Going to Sleep Mode"));
    modem.sendAT(GF("AT+CSCLK=2"));
    modem.waitResponse();
    SerialMon.println(F("Sleep Mode sent"));
}


#endif
