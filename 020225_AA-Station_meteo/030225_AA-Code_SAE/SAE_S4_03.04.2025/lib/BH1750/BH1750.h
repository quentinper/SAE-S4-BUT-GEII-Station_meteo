#ifndef BH_1750_H
#define BH_1750_H

#include <Arduino.h>
#include <Wire.h>
// define
#define BH1750_POWER_DOWN 0x00
#define BH1750_POWER_ON 0x01
#define BH1750_CONTINOUS_H_RES_MOD 0x10
#define BH1750_CONTINOUS_H_RES_MOD_2 0x11
#define BH1750_CONTINOUS_L_RES_MOD 0x13
#define BH1750_ONE_TIME_H_RES_MOD 0x20
#define BH1750_ONE_TIME_H_RES_MOD_2 0x21
#define BH1750_ONE_TIME_L_RES_MOD 0x23

// a compléter
class BH1750
{
public:
    // constructor
    BH1750();
    //
    bool begin(uint8_t BH1750_adr);
    void setMode(uint8_t mode);
    bool isMeasurementReady();
    void powerOn();
    void powerDown();
    float readValue();
    float luminosite_directe();

private:
    uint8_t _adress;
    float _value;
    uint8_t _mode;
    uint32_t _lastTime;
};

#endif