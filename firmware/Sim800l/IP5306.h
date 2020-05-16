/**
 *  https://gist.github.com/me-no-dev/7702f08dd578de5efa47caf322250b57
 */

#include "Arduino.h"
#include "Wire.h"


#define IP5306_ADDR          0x75
#define IP5306_REG_SYS_CTL0  0x00

#define I2C_SDA              21
#define I2C_SCL              22

TwoWire I2CPower = TwoWire(0);

void IP5306_setup() {
  I2CPower.begin(I2C_SDA, I2C_SCL, 400000);
}

bool IP5306_setPowerBoostKeepOn(int en) {
  I2CPower.beginTransmission(IP5306_ADDR);
  I2CPower.write(IP5306_REG_SYS_CTL0);
  if (en) {
    I2CPower.write(0x37); // Set bit1: 1 enable 0 disable boost keep on
  } else {
    I2CPower.write(0x35); // 0x37 is default reg value
  }
  return I2CPower.endTransmission() == 0;
}

/*
** IP5306 Power Module
*/

/* M5 Defaults
KeyOff: Enabled
BoostOutput: Disabled
PowerOnLoad: Enabled
Charger: Enabled
Boost: Enabled
LowBatShutdown: Enabled
ShortPressBoostSwitch: Disabled
FlashlightClicks: Double Press
BoostOffClicks: Long Press
BoostAfterVin: Open
LongPressTime: 2s
ChargeUnderVoltageLoop: 4.55V
ChargeCCLoop: Vin
VinCurrent: 2250mA
VoltagePressure: 28mV
ChargingFullStopVoltage: 4.17V
LightLoadShutdownTime: 32s
EndChargeCurrentDetection: 500mA
ChargeCutoffVoltage: 4.2V
*/

#define IP5306_REG_SYS_0    0x00
#define IP5306_REG_SYS_1    0x01
#define IP5306_REG_SYS_2    0x02
#define IP5306_REG_CHG_0    0x20
#define IP5306_REG_CHG_1    0x21
#define IP5306_REG_CHG_2    0x22
#define IP5306_REG_CHG_3    0x23
#define IP5306_REG_CHG_4    0x24
#define IP5306_REG_READ_0   0x70
#define IP5306_REG_READ_1   0x71
#define IP5306_REG_READ_2   0x72
#define IP5306_REG_READ_3   0x77
#define IP5306_REG_READ_4   0x78

#define IP5306_GetKeyOffEnabled()               ip5306_get_bits(IP5306_REG_SYS_0, 0, 1)
#define IP5306_SetKeyOffEnabled(v)              ip5306_set_bits(IP5306_REG_SYS_0, 0, 1, v)//0:dis,*1:en

#define IP5306_GetBoostOutputEnabled()          ip5306_get_bits(IP5306_REG_SYS_0, 1, 1)
#define IP5306_SetBoostOutputEnabled(v)         ip5306_set_bits(IP5306_REG_SYS_0, 1, 1, v)//*0:dis,1:en

#define IP5306_GetPowerOnLoadEnabled()          ip5306_get_bits(IP5306_REG_SYS_0, 2, 1)
#define IP5306_SetPowerOnLoadEnabled(v)         ip5306_set_bits(IP5306_REG_SYS_0, 2, 1, v)//0:dis,*1:en

#define IP5306_GetChargerEnabled()              ip5306_get_bits(IP5306_REG_SYS_0, 4, 1)
#define IP5306_SetChargerEnabled(v)             ip5306_set_bits(IP5306_REG_SYS_0, 4, 1, v)//0:dis,*1:en

#define IP5306_GetBoostEnabled()                ip5306_get_bits(IP5306_REG_SYS_0, 5, 1)
#define IP5306_SetBoostEnabled(v)               ip5306_set_bits(IP5306_REG_SYS_0, 5, 1, v)//0:dis,*1:en

#define IP5306_GetLowBatShutdownEnable()        ip5306_get_bits(IP5306_REG_SYS_1, 0, 1)
#define IP5306_SetLowBatShutdownEnable(v)       ip5306_set_bits(IP5306_REG_SYS_1, 0, 1, v)//0:dis,*1:en

#define IP5306_GetBoostAfterVin()               ip5306_get_bits(IP5306_REG_SYS_1, 2, 1)
#define IP5306_SetBoostAfterVin(v)              ip5306_set_bits(IP5306_REG_SYS_1, 2, 1, v)//0:Closed, *1:Open

#define IP5306_GetShortPressBoostSwitchEnable() ip5306_get_bits(IP5306_REG_SYS_1, 5, 1)
#define IP5306_SetShortPressBoostSwitchEnable(v) ip5306_set_bits(IP5306_REG_SYS_1, 5, 1, v)//*0:disabled, 1:enabled

#define IP5306_GetFlashlightClicks()            ip5306_get_bits(IP5306_REG_SYS_1, 6, 1)
#define IP5306_SetFlashlightClicks(v)           ip5306_set_bits(IP5306_REG_SYS_1, 6, 1, v)//*0:short press twice, 1:long press

#define IP5306_GetBoostOffClicks()              ip5306_get_bits(IP5306_REG_SYS_1, 7, 1)
#define IP5306_SetBoostOffClicks(v)             ip5306_set_bits(IP5306_REG_SYS_1, 7, 1, v)//*0:long press, 1:short press twice

#define IP5306_GetLightLoadShutdownTime()       ip5306_get_bits(IP5306_REG_SYS_2, 2, 2)
#define IP5306_SetLightLoadShutdownTime(v)      ip5306_set_bits(IP5306_REG_SYS_2, 2, 2, v)//0:8s, *1:32s, 2:16s, 3:64s

#define IP5306_GetLongPressTime()               ip5306_get_bits(IP5306_REG_SYS_2, 4, 1)
#define IP5306_SetLongPressTime(v)              ip5306_set_bits(IP5306_REG_SYS_2, 4, 1, v)//*0:2s, 1:3s

#define IP5306_GetChargingFullStopVoltage()     ip5306_get_bits(IP5306_REG_CHG_0, 0, 2)
#define IP5306_SetChargingFullStopVoltage(v)    ip5306_set_bits(IP5306_REG_CHG_0, 0, 2, v)//0:4.14V, *1:4.17V, 2:4.185V, 3:4.2V (values are for charge cutoff voltage 4.2V, 0 or 1 is recommended)

#define IP5306_GetChargeUnderVoltageLoop()      ip5306_get_bits(IP5306_REG_CHG_1, 2, 3)   //Automatically adjust the charging current when the voltage of VOUT is greater than the set value
#define IP5306_SetChargeUnderVoltageLoop(v)     ip5306_set_bits(IP5306_REG_CHG_1, 2, 3, v)//Vout=4.45V + (v * 0.05V) (default 4.55V) //When charging at the maximum current, the charge is less than the set value. Slowly reducing the charging current to maintain this voltage

#define IP5306_GetEndChargeCurrentDetection()   ip5306_get_bits(IP5306_REG_CHG_1, 6, 2)
#define IP5306_SetEndChargeCurrentDetection(v)  ip5306_set_bits(IP5306_REG_CHG_1, 6, 2, v)//0:200mA, 1:400mA, *2:500mA, 3:600mA

#define IP5306_GetVoltagePressure()             ip5306_get_bits(IP5306_REG_CHG_2, 0, 2)
#define IP5306_SetVoltagePressure(v)            ip5306_set_bits(IP5306_REG_CHG_2, 0, 2, v)//0:none, 1:14mV, *2:28mV, 3:42mV (28mV recommended for 4.2V)

#define IP5306_GetChargeCutoffVoltage()         ip5306_get_bits(IP5306_REG_CHG_2, 2, 2)
#define IP5306_SetChargeCutoffVoltage(v)        ip5306_set_bits(IP5306_REG_CHG_2, 2, 2, v)//*0:4.2V, 1:4.3V, 2:4.35V, 3:4.4V

#define IP5306_GetChargeCCLoop()                ip5306_get_bits(IP5306_REG_CHG_3, 5, 1)
#define IP5306_SetChargeCCLoop(v)               ip5306_set_bits(IP5306_REG_CHG_3, 5, 1, v)//0:BAT, *1:VIN

#define IP5306_GetVinCurrent()                  ip5306_get_bits(IP5306_REG_CHG_4, 0, 5)
#define IP5306_SetVinCurrent(v)                 ip5306_set_bits(IP5306_REG_CHG_4, 0, 5, v)//ImA=(v*100)+50 (default 2250mA)

#define IP5306_GetShortPressDetected()          ip5306_get_bits(IP5306_REG_READ_3, 0, 1)
#define IP5306_ClearShortPressDetected()        ip5306_set_bits(IP5306_REG_READ_3, 0, 1, 1)

#define IP5306_GetLongPressDetected()           ip5306_get_bits(IP5306_REG_READ_3, 1, 1)
#define IP5306_ClearLongPressDetected()         ip5306_set_bits(IP5306_REG_READ_3, 1, 1, 1)

#define IP5306_GetDoubleClickDetected()         ip5306_get_bits(IP5306_REG_READ_3, 2, 1)
#define IP5306_ClearDoubleClickDetected()       ip5306_set_bits(IP5306_REG_READ_3, 2, 1, 1)

#define IP5306_GetPowerSource()                 ip5306_get_bits(IP5306_REG_READ_0, 3, 1)//0:BAT, 1:VIN
#define IP5306_GetBatteryFull()                 ip5306_get_bits(IP5306_REG_READ_1, 3, 1)//0:CHG/DIS, 1:FULL
#define IP5306_GetOutputLoad()                  ip5306_get_bits(IP5306_REG_READ_2, 2, 1)//0:heavy, 1:light
#define IP5306_GetLevelLeds()                ((~ip5306_get_bits(IP5306_REG_READ_4, 4, 4)) & 0x0F)//LED[0-4] State (inverted)

#define IP5306_LEDS2PCT(byte)  \
  ((byte & 0x01 ? 25 : 0) + \
  (byte & 0x02 ? 25 : 0) + \
  (byte & 0x04 ? 25 : 0) + \
  (byte & 0x08 ? 25 : 0))

int ip5306_get_reg(uint8_t reg){
    I2CPower.beginTransmission(0x75);
    I2CPower.write(reg);
    if(I2CPower.endTransmission(false) == 0 && I2CPower.requestFrom(0x75, 1)){
        return I2CPower.read();
    }
    return -1;
}

int ip5306_set_reg(uint8_t reg, uint8_t value){
    I2CPower.beginTransmission(0x75);
    I2CPower.write(reg);
    I2CPower.write(value);
    if(I2CPower.endTransmission(true) == 0){
        return 0;
    }
    return -1;
}

uint8_t ip5306_get_bits(uint8_t reg, uint8_t index, uint8_t bits){
    int value = ip5306_get_reg(reg);
    if(value < 0){
        Serial.printf("ip5306_get_bits fail: 0x%02x\n", reg);
        return 0;
    }
    return (value >> index) & ((1 << bits)-1);
}

void ip5306_set_bits(uint8_t reg, uint8_t index, uint8_t bits, uint8_t value){
    uint8_t mask = (1 << bits) - 1;
    int v = ip5306_get_reg(reg);
    if(v < 0){
        Serial.printf("ip5306_get_reg fail: 0x%02x\n", reg);
        return;
    }
    v &= ~(mask << index);
    v |= ((value & mask) << index);
    if(ip5306_set_reg(reg, v)){
        Serial.printf("ip5306_set_bits fail: 0x%02x\n", reg);
    }
}

void printIP5306Stats(){
    bool usb = IP5306_GetPowerSource();
    bool full = IP5306_GetBatteryFull();
    uint8_t leds = IP5306_GetLevelLeds();
    Serial.printf("IP5306: Power Source: %s, Battery State: %s, Battery Available: %u%%\n", usb?"USB":"BATTERY", full?"CHARGED":(usb?"CHARGING":"DISCHARGING"), IP5306_LEDS2PCT(leds));
}
