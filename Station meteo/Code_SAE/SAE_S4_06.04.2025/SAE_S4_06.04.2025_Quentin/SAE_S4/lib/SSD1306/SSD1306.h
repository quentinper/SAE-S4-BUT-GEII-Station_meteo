#ifndef SSD1306_H
#define SSD1306_H

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <RTC3231.h>
#include <DS1621.h>
#include <BH1750.h>
#include <AHT20.h>

// les registres

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

extern Adafruit_SSD1306 oled;
extern RTC3231 myRTC3231;
extern DS1621 myDS1621;
extern BH1750 myBH1750;
extern AHT20 myAHT20;

class SSD1306
{
    //
public:
    // constructeur
    SSD1306();
    void beginOLED(uint8_t SSD1306_adr);
    void affichage_heures();
    void affichage_menus(int i);
    String affichage_temperature();
    void affichage_droit_auteur();
    void affichage_luminosite();
    void affichage_humidite();

    // méthodes
private:
    uint8_t _adress;
    int _value;
};

#endif