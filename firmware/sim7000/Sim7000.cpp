#ifndef FIRMWARE_SMARTBIKE_SMARTBIKE_CPP_
#define FIRMWARE_SMARTBIKE_SMARTBIKE_CPP_

#include "Sim7000.h"

#define TINY_GSM_MODEM_SIM7000
#define TINY_GSM_RX_BUFFER 1024 // Set RX buffer to 1Kb
#define SerialAT Serial1

/*
   Tests enabled
*/
#define TINY_GSM_TEST_GPRS    true
#define TINY_GSM_TEST_GPS     true
#define TINY_GSM_POWERDOWN    true

// set GSM PIN, if any
#define GSM_PIN ""

// Your GPRS credentials, if any
const char apn[]  = "chili";     //SET TO YOUR APN
const char gprsUser[] = "";
const char gprsPass[] = "";

#define EXTERNAL_URL "bike.x.maxistar.me"
#define EXTERNAL_URL_PORT 80

// Server details
// The server variable can be just a domain name or it can have a subdomain. It depends on the service you are using
const char server[] = EXTERNAL_URL;     // domain name: example.com, maker.ifttt.com, etc
const char resource[] = "/api/status/"; // resource path, for example: /post-data.php
const int  port = EXTERNAL_URL_PORT;    // server port number


#include <TinyGsmClient.h>
#include <SPI.h>
#include <Ticker.h>

TinyGsm modem(SerialAT);
TinyGsmClient client(modem);

#define uS_TO_S_FACTOR 1000000ULL  // Conversion factor for micro seconds to seconds
#define TIME_TO_SLEEP  60 * 30          // Time ESP32 will go to sleep (in seconds)

#define UART_BAUD   9600
#define PIN_DTR     25
#define PIN_TX      27
#define PIN_RX      26
#define PWR_PIN     4

#define LED_PIN     12

int counter, lastIndex, numberOfPieces = 24;
String pieces[24], input;

float batteryVoltage = 0;
float solarVoltage = 0;
float longitude = 0;
float latitude = 0;

#define INPUT_BUFFER_SIZE    1024
char inputBuffer[INPUT_BUFFER_SIZE];


void httpPost(
  String httpRequestData,
  const char* server,
  const char* resource,
  int port
) {
  unsigned long num = 0;
  Serial.println(httpRequestData);

  Serial.print("Connecting to ");
  Serial.print(server);
  if (!client.connect(server, port)) {
      Serial.println(" fail");
  } else {
      Serial.println(" OK");

      // Making an HTTP POST request
      Serial.println("Performing HTTP POST request...");

      client.print(String("POST ") + resource + " HTTP/1.1\r\n");
      client.print(String("Host: ") + server + "\r\n");
      client.println("Connection: close");
      //client->println("Content-Type: application/x-www-form-urlencoded");
      client.println("Content-Type: application/json");
      client.print("Content-Length: ");
      client.println(httpRequestData.length());
      client.println();
      client.println(httpRequestData);

      unsigned long timeout = millis();
      while (client.connected() && millis() - timeout < 10000L) {
        // Print available data (HTTP response from server)
        while (client.available()) {
          char c = client.read();
          Serial.print(c);
          timeout = millis();
          if (num < INPUT_BUFFER_SIZE) {
            inputBuffer[num++] = c;
          }
        }
      }
      inputBuffer[num] = 0;
      Serial.println();

      // Close client and disconnect
      client.stop();
      Serial.println(F("Server disconnected"));
  }
}

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

    httpPost(httpRequestData, server, resource, port);
}

/**
 * Setup
 */
void Sim7000::setup()
{
    // Set console baud rate
    Serial.begin(9600);
    delay(10);

    // Set LED OFF
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, HIGH);

    pinMode(PWR_PIN, OUTPUT);
    digitalWrite(PWR_PIN, HIGH);
    delay(300);
    digitalWrite(PWR_PIN, LOW);

    Serial.println("\nWait...");

    delay(10000);

    SerialAT.begin(UART_BAUD, SERIAL_8N1, PIN_RX, PIN_TX);

    // Restart takes quite some time
    // To skip it, call init() instead of restart()
    Serial.println("Initializing modem...");
    if (!modem.restart()) {
        Serial.println("Failed to restart modem, attempting to continue without restarting");
    }

    String name = modem.getModemName();
    delay(500);
    Serial.println("Modem Name: " + name);

    String modemInfo = modem.getModemInfo();
    delay(500);
    Serial.println("Modem Info: " + modemInfo);

    // Set SIM7000G GPIO4 LOW ,turn off GPS power
    // CMD:AT+SGPIO=0,4,1,0
    // Only in version 20200415 is there a function to control GPS power
    modem.sendAT("+SGPIO=0,4,1,0");

    // Unlock your SIM card with a PIN if needed
    if ( GSM_PIN && modem.getSimStatus() != 3 ) {
        modem.simUnlock(GSM_PIN);
    }

    /*
      2 Automatic
      13 GSM only
      38 LTE only
      51 GSM and LTE only
    * * * */
    String res;
    do {
        res = modem.setNetworkMode(38);
        delay(500);
    } while (res != "OK");

    /*
      1 CAT-M
      2 NB-Iot
      3 CAT-M and NB-IoT
    * * */
    do {
        res = modem.setPreferredMode(1);
        delay(500);
    } while (res != "OK");

    // pinMode(35, ANALOG_INPUT);
    // pinMode(36, ANALOG_INPUT);
}

/**
 * Loop
 */
void Sim7000::loop()
{
  SerialAT.println("AT+CGDCONT?");
  delay(500);
  if (SerialAT.available()) {
    input = SerialAT.readString();
    for (int i = 0; i < input.length(); i++) {
      if (input.substring(i, i + 1) == "\n") {
        pieces[counter] = input.substring(lastIndex, i);
        lastIndex = i + 1;
        counter++;
      }
      if (i == input.length() - 1) {
        pieces[counter] = input.substring(lastIndex, i);
      }
    }
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
              modem.sendAT("+CGDCONT=" + String(data.toInt()) + ",\"IP\",\"" + String(apn) + "\",\"0.0.0.0\",0,0,0,0");
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
    Serial.println("Failed to get PDP!");
  }


  Serial.println("\n\n\nWaiting for network...");
  if (!modem.waitForNetwork()) {
    delay(10000);
    return;
  }

  if (modem.isNetworkConnected()) {
    Serial.println("Network connected");
  }

  Serial.println("\n---Starting GPRS TEST---\n");
  Serial.println("Connecting to: " + String(apn));
  if (!modem.gprsConnect(apn, gprsUser, gprsPass)) {
    delay(10000);
    return;
  }

  Serial.print("GPRS status: ");
  if (modem.isGprsConnected()) {
    Serial.println("connected");
  } else {
    Serial.println("not connected");
  }

  String ccid = modem.getSimCCID();
  Serial.println("CCID: " + ccid);

  String imei = modem.getIMEI();
  Serial.println("IMEI: " + imei);

  String cop = modem.getOperator();
  Serial.println("Operator: " + cop);

  IPAddress local = modem.localIP();
  Serial.println("Local IP: " + String(local));

  int csq = modem.getSignalQuality();
  Serial.println("Signal quality: " + String(csq));

  SerialAT.println("AT+CPSI?");     //Get connection type and band
  delay(500);
  if (SerialAT.available()) {
    String r = SerialAT.readString();
    Serial.println(r);
  }

  Serial.println("\n---End of GPRS TEST---\n");

  solarVoltage = analogRead(36);
  batteryVoltage = analogRead(35);

  Serial.println("\n---Starting GPS TEST---\n");
  // Set SIM7000G GPIO4 HIGH ,turn on GPS power
  // CMD:AT+SGPIO=0,4,1,1
  // Only in version 20200415 is there a function to control GPS power
  modem.sendAT("+SGPIO=0,4,1,1");

  modem.enableGPS();
  while (1) {
    if (modem.getGPS(&latitude, &longitude)) {
      Serial.printf("lat:%f lon:%f\n", latitude, longitude);
      break;
    } else {
      Serial.print("getGPS ");
      Serial.println(millis());
    }
    delay(2000);
  }
  modem.disableGPS();

  // Set SIM7000G GPIO4 LOW ,turn off GPS power
  // CMD:AT+SGPIO=0,4,1,0
  // Only in version 20200415 is there a function to control GPS power
  modem.sendAT("+SGPIO=0,4,1,0");
  Serial.println("\n---End of GPRS TEST---\n");

  sendState();

  modem.gprsDisconnect();
  if (!modem.isGprsConnected()) {
    Serial.println("GPRS disconnected");
  } else {
    Serial.println("GPRS disconnect: Failed.");
  }

  modem.poweroff();
  Serial.println("Poweroff.");

  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
  delay(200);
  esp_deep_sleep_start();

  // Do nothing forevermore
  while (true) {
    modem.maintain();
  }
}


#endif  // FIRMWARE_SMARTBIKE_SMARTBIKE_CPP_
