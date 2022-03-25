#ifndef GPS_TRACKER_CPP
#define GPS_TRACKER_CPP

// Copyright 2020 Max Starikov


#include "./Arduino.h"
#include "./GPSTracker.h"

#include "config_common.h"


#define SerialGSM Serial2

uint8_t config1[] = {
  // Disable NMEA
  0xB5,0x62,0x06,0x01,0x08,0x00,0xF0,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x24, // GxGGA off
  0xB5,0x62,0x06,0x01,0x08,0x00,0xF0,0x01,0x00,0x00,0x00,0x00,0x00,0x01,0x01,0x2B, // GxGLL off
  0xB5,0x62,0x06,0x01,0x08,0x00,0xF0,0x02,0x00,0x00,0x00,0x00,0x00,0x01,0x02,0x32, // GxGSA off
  0xB5,0x62,0x06,0x01,0x08,0x00,0xF0,0x03,0x00,0x00,0x00,0x00,0x00,0x01,0x03,0x39, // GxGSV off
  0xB5,0x62,0x06,0x01,0x08,0x00,0xF0,0x04,0x00,0x00,0x00,0x00,0x00,0x01,0x04,0x40, // GxRMC off
  0xB5,0x62,0x06,0x01,0x08,0x00,0xF0,0x05,0x00,0x00,0x00,0x00,0x00,0x01,0x05,0x47, // GxVTG off

  
  // Enable UBX
  0xB5,0x62,0x06,0x01,0x08,0x00,0x01,0x07,0x00,0x01,0x00,0x00,0x00,0x00,0x18,0xE1, //NAV-PVT on
  0xB5,0x62,0x06,0x01,0x08,0x00,0x01,0x02,0x00,0x01,0x00,0x00,0x00,0x00,0x13,0xBE, //NAV-POSLLH on
  0xB5,0x62,0x06,0x01,0x08,0x00,0x01,0x03,0x00,0x01,0x00,0x00,0x00,0x00,0x14,0xC5, //NAV-STATUS on

  0xB5,0x62,0x06,0x08,0x06,0x00,0xE8,0x03,0x01,0x00,0x01,0x00,0x01,0x39, //(1Hz)
};

byte gps_set_sucess = 0;


GPSTracker::GPSTracker() {

}

// Send a byte array of UBX protocol to the GPS
void sendUBX(uint8_t *MSG, uint8_t len) {
  for(int i= 0; i < len; i++) {
    SerialGSM.write(MSG[i]);
    Serial.print(MSG[i], HEX);
  }
  SerialGSM.println();
}

void powerOff() {
  uint8_t setPowerSaveMode[] = { 0xB5, 0x62, 0x02, 0x41, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x4D, 0x3B };
  sendUBX(setPowerSaveMode, sizeof(setPowerSaveMode)/sizeof(uint8_t));
}

void wakeUp() {
  uint8_t setUp[] = { 0x00 };
  sendUBX(setUp, sizeof(setUp)/sizeof(uint8_t));
  delay(500);
}

// Calculate expected UBX ACK packet and parse UBX response from GPS
boolean getUBX_ACK(uint8_t *MSG) {
  uint8_t b;
  uint8_t ackByteID = 0;
  uint8_t ackPacket[10];
  unsigned long startTime = millis();
  Serial.print(" * Reading ACK response: ");

  // Construct the expected ACK packet   
  ackPacket[0] = 0xB5;  // header
  ackPacket[1] = 0x62;  // header
  ackPacket[2] = 0x05;  // class
  ackPacket[3] = 0x01;  // id
  ackPacket[4] = 0x02;  // length
  ackPacket[5] = 0x00;
  ackPacket[6] = MSG[2];  // ACK class
  ackPacket[7] = MSG[3];  // ACK id
  ackPacket[8] = 0;   // CK_A
  ackPacket[9] = 0;   // CK_B

  // Calculate the checksums
  for (uint8_t i = 2; i < 8; i++) {
    ackPacket[8] = ackPacket[8] + ackPacket[i];
    ackPacket[9] = ackPacket[9] + ackPacket[8];
  }

  while (1) {

    // Test for success
    if (ackByteID > 9) {
      // All packets in order!
      Serial.println(" (SUCCESS!)");
      return true;
    }

    // Timeout if no valid response in 3 seconds
    if (millis() - startTime > 3000) {
      Serial.println(" (FAILED!)");
      return false;
    }

    // Make sure data is available to read
    if (SerialGSM.available()) {
      b = SerialGSM.read();

      // Check that bytes arrive in sequence as per expected ACK packet
      if (b == ackPacket[ackByteID]) {
        ackByteID++;
        SerialGSM.print(b, HEX);
      } else {
        ackByteID = 0;  // Reset and look again, invalid order
      }

    }
  }
}



void GPSTracker::setup() {
  pinMode(GSM_RX, INPUT);
  pinMode(GSM_TX, INPUT);
  // Set GSM module baud rate and UART pins
  SerialGSM.begin(9600, SERIAL_8N1, GSM_RX, GSM_TX);
  //if it sleaps wake it up
  
  wakeUp();

  //configure module
  gps_set_sucess = 0;
  int num_retries = 0;
  while(!gps_set_sucess && num_retries++ < 5) {
      Serial.write("Try to switch off NMEA\n");
      sendUBX(config1, sizeof(config1)/sizeof(uint8_t));
      gps_set_sucess = getUBX_ACK(config1);
      delay(1000);
  }
  //send to sleep again!
  powerOff();
}

void GPSTracker::forceSleep() {
    wakeUp();
    delay(1000);
    powerOff();
}

void GPSTracker::loop() {
    while (SerialGSM.available() > 0){
    // get the byte data from the GPS
      byte gpsData = SerialGSM.read();
      Serial.write(gpsData);
    }
}




#endif //GPS_TRACKER_CPP
