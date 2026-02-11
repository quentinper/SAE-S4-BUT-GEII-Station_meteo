#include <Arduino.h>
#include <Wire.h>
#include <BH1750.h>

//
float theLuxValue = 0.0f;
uint8_t theMode = 0x00;
uint32_t lastTime = 0;

// a compléter...
BH1750::BH1750()
{
    _adress = 0;
    _mode = 0;
}
bool BH1750::begin(uint8_t adress)
{
    _adress = adress;
    bool state = false;
    Wire.begin();
    Wire.beginTransmission(_adress);
    if (Wire.endTransmission() == 0)
    {
        Serial.println("Capteur BH1750 connecté.");
        return true;
    }
    else
    {
        Serial.println("Échec de connexion au capteur BH1750 !");
        return false;
    }
    return state;
}
void BH1750::setMode(uint8_t mode)
{
    _mode = mode;
    Wire.beginTransmission(_adress);
    Wire.write(_mode);
    Wire.endTransmission();
}
bool BH1750::isMeasurementReady()
{
    uint32_t timedelay = 0;
    bool res = false;
    uint32_t now = 0;
    switch (_mode)
    {
    case BH1750_CONTINOUS_H_RES_MOD:
        timedelay = 180;
        break;
    case BH1750_CONTINOUS_H_RES_MOD_2:
        timedelay = 180;
        break;
    case BH1750_CONTINOUS_L_RES_MOD:
        timedelay = 180;
        break;
    case BH1750_ONE_TIME_H_RES_MOD:
        timedelay = 24;
        break;
    case BH1750_ONE_TIME_H_RES_MOD_2:
        timedelay = 24;
        break;
    case BH1750_ONE_TIME_L_RES_MOD:
        timedelay = 24;
        break;
    default:
        timedelay = 180;
        break;
    }
    now = millis();
    if ((now - _lastTime) > timedelay)
        res = true;
    return res;
}
void BH1750::powerOn()
{
    Wire.beginTransmission(_adress);
    Wire.write(BH1750_POWER_ON);
    Wire.endTransmission();
}
void BH1750::powerDown()
{
    Wire.beginTransmission(_adress);
    Wire.write(BH1750_POWER_DOWN);
    Wire.endTransmission();
}

float BH1750::readValue()
{
    uint16_t data = 0x0000;
    uint8_t upper = 0x00;
    uint8_t lower = 0x00;
    float res = 0;

    Wire.requestFrom(_adress, byte(2));
    upper = Wire.read();
    lower = Wire.read();
    Wire.endTransmission();

    data = (upper << 8 | lower);
    res = data / 1.20f;
    _lastTime = millis();
    return res;
}

float BH1750::luminosite_directe()
{
    float lux = 0;
    if (isMeasurementReady())
    {
        lux = readValue();
    }
    return lux;
}