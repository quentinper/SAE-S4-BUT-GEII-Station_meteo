#ifndef CONF_WIFI_H
#define CONF_WIFI_H

#include <Arduino.h>
#include <Wire.h>
#include <WiFi.h>
#include <RTC3231.h>
#include <DS1621.h>
#include <BH1750.h>
#include <AHT20.h>
#include <SSD1306.h>
// define
extern RTC3231 myRTC3231;
extern DS1621 myDS1621;
extern BH1750 myBH1750;
extern AHT20 myAHT20;
extern SSD1306 mySSD1306;
// a compléter
class CONFWIFI
{
public:
    // constructor
    CONFWIFI();
    void set_routes();
    bool wifi_begin(const char *ssid, const char *password);
    void initLittleFS();
    void get_value();
    void get_state(bool state_DS1621,bool state_RTC3231,bool state_BH1750,bool state_AHT20,bool state_SDCard,bool state_WIFI);
    //

private:
    const char *_ssid;
    const char *_password;
    
};

#endif