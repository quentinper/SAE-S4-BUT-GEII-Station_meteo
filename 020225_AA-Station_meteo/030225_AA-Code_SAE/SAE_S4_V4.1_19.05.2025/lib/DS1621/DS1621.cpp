#include <DS1621.h>
#include <Wire.h>
#include <Arduino.h>

DS1621::DS1621()
{
    _adress = 0;
    _temperature = 0;
}

//************************************
bool DS1621::begin(uint8_t adress)
{
    bool state = false;
    _adress = adress; // chaque objet aura sa propre adresse
    Wire.begin();
    Wire.beginTransmission(_adress);
    if (Wire.endTransmission() == 0)
    {
        Serial.println("Capteur DS1621 connecté.");
        return true;
    }
    else
    {
        Serial.println("Échec de connexion au capteur DS1621 !");
        return false;
    }
    return state;
}
//***************************************
float DS1621::getValue()
{
    uint8_t tmsb, tlsb;
    int16_t temperature_brut;
    float temperature;

    // Envoyer la commande de lecture
    Wire.beginTransmission(_adress);
    Wire.write(DS1621_Read_Temperature);
    Wire.endTransmission();

    // Demande de lecture de 2 octets
    Wire.requestFrom(_adress, (uint8_t)2);
    if (Wire.available() == 2)
    {
        tmsb = Wire.read();
        tlsb = Wire.read();
    }
    else
    {
        Serial.println("Erreur : pas assez de données reçues !");
        return -404; // Valeur d'erreur
    }

    // Traitement des données
    temperature_brut = (int8_t)tmsb;                           // MSB est déjà en complément à deux
    temperature = (float)temperature_brut + (tlsb >> 7) * 0.5; // Ajout du 0.5°C si nécessaire
    return temperature;
}

void DS1621::startConv()
{
    Wire.beginTransmission(_adress);
    Wire.write(DS1621_start_reg);
    Wire.endTransmission();
}

void DS1621::stopConv()
{
    Wire.beginTransmission(_adress);
    Wire.write(DS1621_stop_reg);
    Wire.endTransmission();
    Serial.println("Arrêt de la conversion de température.");
}