/***************************************************************
 * @file        CMDUSER.cpp
 * @author      Lorenzo S. & Quentin P.
 * @version     2
 * @date        2025-05-19
 * @brief       Gestion des commandes utilisateur.
 * @details     SAE S4 BUG GEII FA ESE
 * @copyright   (c) Lorenzo S. & Quentin P.- Tous droits réservés
 ***************************************************************/
#include <Arduino.h>
#include <Wire.h>
#include <CMDUSER.h>

CMDUSER::CMDUSER()
{
    _BUTTON_UP_PIN = 0;
    _BUTTON_SELECT_PIN = 0;
    _BUTTON_DOWN_PIN = 0;
}

const int maxIndex = 3;
int i = 0;
int value = 0;

void CMDUSER::commande_utilisateur(uint8_t BUTTON_UP_PIN, uint8_t BUTTON_SELECT_PIN, uint8_t BUTTON_DOWN_PIN)
{
    _BUTTON_UP_PIN = BUTTON_UP_PIN;
    _BUTTON_SELECT_PIN = BUTTON_SELECT_PIN;
    _BUTTON_DOWN_PIN = BUTTON_DOWN_PIN;
    if (digitalRead(BUTTON_UP_PIN) == HIGH)
    {
        i++;
        Serial.println("UP");
        Serial.println(i);
        if (i > maxIndex)
            i = 0;
        delay(200);
    }
    else if (digitalRead(BUTTON_DOWN_PIN) == HIGH)
    {
        i--;
        Serial.println("DOWN");
        Serial.println(i);
        if (i < 0)
            i = maxIndex;
        delay(200);
    }
    else if (digitalRead(BUTTON_SELECT_PIN) == HIGH)
    {
        if (i == 0)
        {
            value = 1;
            delay(200);
        }
        else if (i == 1)
        {
            value = 2;
            delay(200);
        }
        else if (i == 2)
        {
            value = 3;
            delay(200);
        }
        else if (i == 3)
        {
            value = 4;
            delay(200);
        }
    }
}
void CMDUSER::affichage_ecran()
{
    if (value != 1 && value != 2 && value != 3)
    {
        mySSD1306.affichage_menus(i);
    }
    switch (value)
    {
    case 1:
        mySSD1306.affichage_heures();
        value = 0;
        break;
    case 2:
        mySSD1306.affichage_temperature();
        value = 0;
        break;
    case 3:
        mySSD1306.affichage_luminosite();
        value = 0;
        break;
    case 4:
        mySSD1306.affichage_humidite();
        value = 0;
        break;
    default:
        break;
    }
}