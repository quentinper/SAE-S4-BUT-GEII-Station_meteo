#ifndef CMDUSER_H
#define CMDUSER_H

#include <Arduino.h>
#include <Wire.h>
#include <SSD1306.h>

extern SSD1306 mySSD1306;

class CMDUSER
{
public:
    CMDUSER();
    void commande_utilisateur(uint8_t BUTTON_UP_PIN,uint8_t BUTTON_SELECT_PIN, uint8_t BUTTON_DOWN_PIN);
    void affichage_ecran();

private:
    uint8_t _BUTTON_UP_PIN;
    uint8_t _BUTTON_SELECT_PIN;
    uint8_t _BUTTON_DOWN_PIN;
};

#endif