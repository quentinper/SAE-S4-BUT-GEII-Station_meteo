/***************************************************************
 * @file        AHT20.h
 * @author      Lorenzo S. & Quentin P.
 * @version     2
 * @date        2025-05-19
 * @brief       Gestion du capteur d'humidité.
 * @details     SAE S4 BUG GEII FA ESE
 * @copyright   (c) Lorenzo S. & Quentin P.- Tous droits réservés
 ***************************************************************/
#ifndef AHT_20_H
#define AHT_20_H

#include <Arduino.h>
#include <Wire.h>

#define AHT20_CMD_INIT_PARAMS_2ND 0x00        ///< The second parameter of init command: 0x00
#define AHT20_CMD_MEASUREMENT_PARAMS_2ND 0x00 ///< The second parameter of trigger measurement command: 0x00
#define AHT20_CMD_INIT_PARAMS_1ST 0x08        ///< The first parameter of init command: 0x08
#define AHT20_CMD_MEASUREMENT_PARAMS_1ST 0x33 ///< The first parameter of trigger measurement command: 0x33
#define AHT20_CMD_STATUS 0x71                 ///< Get status word command
#define AHT20_CMD_MEASUREMENT 0xAC            ///< Trigger measurement command
#define AHT20_CMD_INIT 0xBE                   ///< Init command
#define AHT20_CMD_MEASUREMENT_TIME 80         ///< Measurement command completion time: 80ms
#define AHT20_CMD_POWER_ON_TIME 40            ///< Waiting time for powering on : 40ms
#define AHT20_CMD_INIT_TIME 10                ///< Waiting time for init completion: 10ms

class AHT20
{
public:
    AHT20();
    bool begin(uint8_t AHT20_ADR);
    bool getValues();
    float humidity();
    float temperature();

private:
    float _humidity;
    float _temperature;
    uint8_t _adress;
    bool _start();
};

#endif