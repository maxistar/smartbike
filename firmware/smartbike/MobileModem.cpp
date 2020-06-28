#ifndef MOBILE_MODEM_CPP
#define MOBILE_MODEM_CPP

// Copyright 2020 Max Starikov


#include "./Arduino.h"
#include "./MobileModem.h"
#include "utilities.h"


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

#define INPUT_BUFFER_SIZE    1024
char inputBuffer[INPUT_BUFFER_SIZE];


// Set serial for debug console (to Serial Monitor, default speed 115200)
#define SerialMon Serial
// Set serial for AT commands (to SIM800 module)
#define SerialAT Serial1



// Define the serial console for debug prints, if needed
// #define DUMP_AT_COMMANDS



#include <TinyGsmClient.h>



MobileModem::MobileModem() {
  modem = new TinyGsm(SerialAT);
  client = new TinyGsmClient(*modem);
}

MobileModem::~MobileModem() {
  delete client;
  delete modem;
}

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
  modem->restart();
  // use modem.init() if you don't need the complete restart

  // Unlock your SIM card with a PIN if needed
  if (strlen(simPIN) && modem->getSimStatus() != 3) {
    modem->simUnlock(simPIN);
  }
}

TinyGsm *MobileModem::getModem() {
  return modem;
}

void MobileModem::httpPost(
  String httpRequestData,
  const char* server,
  const char* resource,
  int port
) {
  unsigned long num = 0;
  SerialMon.print("Connecting to APN: ");
  SerialMon.print(apn);
  if (!modem->gprsConnect(apn, gprsUser, gprsPass)) {
    SerialMon.println(" fail");
  } else {
    SerialMon.println(" OK");

    SerialMon.print("Connecting to ");
    SerialMon.print(server);
    if (!client->connect(server, port)) {
      SerialMon.println(" fail");
    } else {
      SerialMon.println(" OK");

      // Making an HTTP POST request
      SerialMon.println("Performing HTTP POST request...");

      client->print(String("POST ") + resource + " HTTP/1.1\r\n");
      client->print(String("Host: ") + server + "\r\n");
      client->println("Connection: close");
      //client->println("Content-Type: application/x-www-form-urlencoded");
      client->println("Content-Type: application/json");
      client->print("Content-Length: ");
      client->println(httpRequestData.length());
      client->println();
      client->println(httpRequestData);

      unsigned long timeout = millis();
      while (client->connected() && millis() - timeout < 10000L) {
        // Print available data (HTTP response from server)
        while (client->available()) {
          char c = client->read();
          SerialMon.print(c);
          timeout = millis();
          if (num < INPUT_BUFFER_SIZE) {
            inputBuffer[num++] = c;
          }
        }
      }
      inputBuffer[num] = 0;
      SerialMon.println();

      // Close client and disconnect
      client->stop();
      SerialMon.println(F("Server disconnected"));
      modem->gprsDisconnect();
      SerialMon.println(F("GPRS disconnected"));
    }
  }
}

String MobileModem::httpGet(
  const char* server,
  const char* resource,
  int port
) {
  unsigned long num = 0;
  inputBuffer[num] = 0;
  SerialMon.print("Connecting to APN: ");
  SerialMon.print(apn);
  if (!modem->gprsConnect(apn, gprsUser, gprsPass)) {
    SerialMon.println(" fail");
  } else {
    SerialMon.println(" OK");

    SerialMon.print("Connecting to ");
    SerialMon.print(server);
    if (!client->connect(server, port)) {
      SerialMon.println(" fail");
    } else {
      SerialMon.println(" OK");

      // Making an HTTP POST request
      SerialMon.println("Performing HTTP GET request...");

      client->print(String("GET ") + resource + " HTTP/1.1\r\n");
      client->print(String("Host: ") + server + "\r\n");
      client->println("Connection: close\r\n\r\n");
      client->println();

      unsigned long timeout = millis();
      bool outputStarted = false;
      char lastChars[4];
      unsigned long bufferPos = 0;
      while (client->connected() && millis() - timeout < 10000L) {
        // Print available data (HTTP response from server)
        while (client->available()) {
          char c = client->read();
          
          SerialMon.print(c);
          timeout = millis();
          if (num < INPUT_BUFFER_SIZE && outputStarted) {
            inputBuffer[num++] = c;
          }

          //test for start of the output
          lastChars[0] = lastChars[1];
          lastChars[1] = lastChars[2];
          lastChars[2] = lastChars[3];
          lastChars[3] = c;
       
          if (!outputStarted && bufferPos > 2 && lastChars[0]=='\r' && lastChars[1]=='\n' && lastChars[2]=='\r' && lastChars[3]=='\n') {
            outputStarted = true;
          }
          bufferPos++;
        }
      }
      inputBuffer[num] = 0;
      SerialMon.println();

      // Close client and disconnect
      client->stop();
      SerialMon.println(F("Server disconnected"));
      modem->gprsDisconnect();
      SerialMon.println(F("GPRS disconnected"));
    }
  }
  return String(inputBuffer);
}

void MobileModem::sleepMode() {
    SerialMon.println(F("Going to Sleep Mode"));
    modem->sendAT(GF("AT+CSCLK=2"));
    modem->waitResponse();
    SerialMon.println(F("Sleep Mode sent"));
}


#endif
