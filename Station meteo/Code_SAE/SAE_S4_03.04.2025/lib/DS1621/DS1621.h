#ifndef DS1621_H
#define DS1621_H

#include <Arduino.h>
#include <Wire.h>
// les registres

#define DS1621_start_reg 0xEE
#define DS1621_Read_Temperature 0xAA
#define DS1621_stop_reg 0x22
#define DS1621_config 0xAC

class DS1621
{
    //
public:
    // constructeur
    DS1621();

    // méthodes
    bool begin(uint8_t DS1621_adr);
    float getValue();
    void startConv();
    void stopConv();

private:
    uint8_t _adress;
    float _temperature;
};

#endif