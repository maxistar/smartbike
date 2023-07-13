#ifndef MOBILE_MODEM_CPP
#define MOBILE_MODEM_CPP

// Copyright 2020 Max Starikov

#include "config.h"

#define SerialMon Serial
#define SerialAT Serial1

#include "./Arduino.h"
#include "./MobileModem.h"
#include <TinyGsmClient.h>

#define INPUT_BUFFER_SIZE    1024
char inputBuffer[INPUT_BUFFER_SIZE];

// Your GPRS credentials, if any
const char apn[]  = "chili";     //SET TO YOUR APN
const char gprsUser[] = "";
const char gprsPass[] = "";

MobileModem::MobileModem() {
  modem = new TinyGsm(SerialAT);
  client = new TinyGsmClient(*modem);
}

MobileModem::~MobileModem() {
  delete client;
  delete modem;
}

void MobileModem::setup() {
    // Restart takes quite some time
    // To skip it, call init() instead of restart()
    SerialMon.println("Initializing modem...");
    if (!modem->restart()) {
        SerialMon.println("Failed to restart modem, attempting to continue without restarting");
    }

    String name = modem->getModemName();
    delay(500);
    SerialMon.println("Modem Name: " + name);

    String modemInfo = modem->getModemInfo();
    delay(500);
    SerialMon.println("Modem Info: " + modemInfo);

    // Set SIM7000G GPIO4 LOW ,turn off GPS power
    // CMD:AT+SGPIO=0,4,1,0
    // Only in version 20200415 is there a function to control GPS power
    modem->sendAT("+SGPIO=0,4,1,0");

    // Unlock your SIM card with a PIN if needed
    if ( GSM_PIN && modem->getSimStatus() != 3 ) {
        modem->simUnlock(GSM_PIN);
    }

    /*
      2 Automatic
      13 GSM only
      38 LTE only
      51 GSM and LTE only
    * * * */
    String res;
    do {
        res = modem->setNetworkMode(38);
        delay(500);
    } while (res != "OK");

    /*
      1 CAT-M
      2 NB-Iot
      3 CAT-M and NB-IoT
    * * */
    do {
        res = modem->setPreferredMode(1);
        delay(500);
    } while (res != "OK");  
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
  SerialMon.println(httpRequestData);

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
  }
  return String(inputBuffer);  
}

void MobileModem::connectNetwork() {
  int counter, lastIndex, numberOfPieces = 24;
  String pieces[24], input;


  SerialMon.println("send: AT+CGDCONT?");
  
  SerialAT.println("AT+CGDCONT?");

  SerialMon.println("delay");

  delay(500);

  SerialMon.println("after delay");
  
  if (SerialAT.available()) {
    SerialMon.println("SerialAT.available()");
    input = SerialAT.readString();
    SerialMon.println("before cycle");
    for (int i = 0; i < input.length(); i++) {
      //SerialMon.println("step1");
      if (input.substring(i, i + 1) == "\n") {
        pieces[counter] = input.substring(lastIndex, i);
        lastIndex = i + 1;
        counter++;
      }
      if (i == input.length() - 1) {
        pieces[counter] = input.substring(lastIndex, i);
      }
    }
    SerialMon.println("second cycle");
    // Reset for reuse
    input = "";
    counter = 0;
    lastIndex = 0;

    for ( int y = 0; y < numberOfPieces; y++) {
      for ( int x = 0; x < pieces[y].length(); x++) {
        char c = pieces[y][x];  //gets one byte from buffer
        if (c == ',') {
          if (input.indexOf(": ") >= 0) {
            String data = input.substring((input.indexOf(": ") + 1));
            if ( data.toInt() > 0 && data.toInt() < 25) {
              modem->sendAT("+CGDCONT=" + String(data.toInt()) + ",\"IP\",\"" + String(apn) + "\",\"0.0.0.0\",0,0,0,0");
            }
            input = "";
            break;
          }
          // Reset for reuse
          input = "";
        }
        else {
          input += c;
        }
      }
    }
  } else {
    SerialMon.println("Failed to get PDP!");
  }


  SerialMon.println("\n\n\nWaiting for network...");
  if (!modem->waitForNetwork()) {
    delay(10000);
    return;
  }

  if (modem->isNetworkConnected()) {
    SerialMon.println("Network connected");
  }

  SerialMon.println("\n---Starting GPRS TEST---\n");
  SerialMon.println("Connecting to: " + String(apn));
  if (!modem->gprsConnect(apn, gprsUser, gprsPass)) {
    delay(10000);
    return;
  }

  
  SerialMon.print("GPRS status: ");
  if (modem->isGprsConnected()) {
    SerialMon.println("connected");
  } else {
    SerialMon.println("not connected");
  }

  String ccid = modem->getSimCCID();
  SerialMon.println("CCID: " + ccid);

  String imei = modem->getIMEI();
  SerialMon.println("IMEI: " + imei);

  String cop = modem->getOperator();
  SerialMon.println("Operator: " + cop);

  IPAddress local = modem->localIP();
  SerialMon.println("Local IP: " + String(local));

  int csq = modem->getSignalQuality();
  SerialMon.println("Signal quality: " + String(csq));

  SerialAT.println("AT+CPSI?");     //Get connection type and band
  delay(500);
  if (SerialAT.available()) {
    String r = SerialAT.readString();
    SerialMon.println(r);
  }

  SerialMon.println("\n---End of GPRS TEST---\n");

}

void MobileModem::sleepMode() {
  
}

#endif
