#ifndef GPS_TRACKER_CPP
#define GPS_TRACKER_CPP

// Copyright 2020 Max Starikov


#include "./Arduino.h"
#include "./GPSTracker.h"

#define GSM_TX 12
#define GSM_RX 25

#define SerialGSM Serial2

byte gps_set_sucess = 0;


GPSTracker::GPSTracker() {

}

// Send a byte array of UBX protocol to the GPS
void sendUBX(uint8_t *MSG, uint8_t len) {
  for(int i=0; i<len; i++) {
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

void GPSTracker::setup() {
  // Set GSM module baud rate and UART pins
  SerialGSM.begin(9600, SERIAL_8N1, GSM_RX, GSM_TX);
  //if it sleaps wake it up
  wakeUp();
  //send to sleep again!
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
