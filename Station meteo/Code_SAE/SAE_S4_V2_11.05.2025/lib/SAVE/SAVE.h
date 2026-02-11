#ifndef SAVE_H
#define SAVE_H

#include <Arduino.h>
#include <Wire.h>
#include <WiFi.h>
#include <RTC3231.h>
#include <DS1621.h>
#include <BH1750.h>
#include <AHT20.h>
#include <SSD1306.h>
#include <SD.h>
#include <SPI.h>
#include <FS.h>

extern RTC3231 myRTC3231;
extern DS1621 myDS1621;
extern BH1750 myBH1750;
extern AHT20 myAHT20;
extern SSD1306 mySSD1306;

class SAVE
{
public:
    // constructor
    SAVE();
    bool SD_Card_begin(uint8_t SD_SCLK,uint8_t SD_MISO,uint8_t SD_MOSI,uint8_t SD_CS);
    void save_on_SD_card();
    void get_value_at_save();
    //

private:
    uint8_t _SD_SCLK, _SD_MISO, _SD_MOSI, _SD_CS;
};

#endif