/***************************************************************
 * @file        AHT20.cpp
 * @author      Lorenzo S. & Quentin P.
 * @version     2
 * @date        2025-05-19
 * @brief       Gestion du capteur d'humidité.
 * @details     SAE S4 BUG GEII FA ESE
 * @copyright   (c) Lorenzo S. & Quentin P.- Tous droits réservés
 ***************************************************************/
#include <Arduino.h>
#include <Wire.h>
#include <AHT20.h>

AHT20::AHT20()
{
    _adress = 0;
    _humidity = 0;
    _temperature = 0;
}

bool AHT20::begin(uint8_t adress)
{
    _adress = adress;
    bool state = false;
    uint8_t calibrationStatus = 0xFF;

    Wire.begin();
    delay(AHT20_CMD_POWER_ON_TIME);

    Wire.beginTransmission(_adress);
    if (Wire.endTransmission() == 0)
    {
        Serial.println("Capteur AHT20 connecté.");

        Wire.beginTransmission(_adress);
        Wire.write(AHT20_CMD_STATUS);
        Wire.endTransmission();

        Wire.requestFrom(_adress, byte(1));
        calibrationStatus = Wire.read();
        Wire.endTransmission();

        if (calibrationStatus & 0b00001000 == 0)
        {
            Wire.beginTransmission(_adress);
            Wire.write(AHT20_CMD_INIT);
            Wire.write(AHT20_CMD_INIT_PARAMS_1ST);
            Wire.write(AHT20_CMD_INIT_PARAMS_2ND);
            Wire.endTransmission();

            delay(AHT20_CMD_INIT_TIME);
        }

        Wire.beginTransmission(_adress);
        Wire.write(0xBE);
        Wire.endTransmission();

        return true;
    }
    else
    {
        Serial.println("Échec de connexion au capteur AHT20 !");
        return false;
    }
    return state;
}

bool AHT20::_start()
{
    uint8_t isBusy = 0xFF;
    uint32_t now = 0;
    Wire.beginTransmission(_adress);
    Wire.write(AHT20_CMD_MEASUREMENT);
    Wire.write(AHT20_CMD_MEASUREMENT_PARAMS_1ST);
    Wire.write(AHT20_CMD_MEASUREMENT_PARAMS_2ND);
    Wire.endTransmission();

    delay(80);

    now = millis();

    do
    {
        if (millis() - now > 200)
        {
            return false;
        }

        Wire.beginTransmission(_adress);
        Wire.write(AHT20_CMD_STATUS);
        Wire.endTransmission();

        Wire.requestFrom(_adress, byte(1));
        isBusy = Wire.read();
        Wire.endTransmission();

        delay(1);
    } while (isBusy & 0x80);

    return true;
}

bool AHT20::getValues()
{
    bool res = true;
    uint8_t values[7] = {0, 0, 0, 0, 0, 0, 0};
    uint8_t index = 0;
    uint32_t temp = 0;
    res = _start();

    if (res == false)
    {
        return false;
    }

    Wire.requestFrom(_adress, byte(7));
    while (Wire.available())
    {
        values[index] = Wire.read();
        index++;
    }
    Wire.endTransmission();

    temp = values[1];
    temp <<= 8;
    temp |= values[2];
    temp <<= 4;
    temp |= (values[3] >> 4);
    _humidity = (temp * 100.0f) / 1048576;

    temp = values[3] & 0x0F;
    temp <<= 8;
    temp |= values[4];
    temp <<= 8;
    temp |= values[5];
    _temperature = (temp * 200.0f) / 1048576 - 50;

    return true;
}

float AHT20::humidity()
{
    float humidity = 0.0f;
    getValues();
    humidity = _humidity;
    return humidity;
}

float AHT20::temperature()
{
    float temperature = 0.0f;
    getValues();
    temperature = _temperature;
    return temperature;
}