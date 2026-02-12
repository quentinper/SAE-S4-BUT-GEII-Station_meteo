#include <Arduino.h>
#include <Wire.h>
#include <SAVE.h>
#include <SD.h>
#include <SPI.h>
#include <FS.h>

//
String _temperatureValue;
String _humiditeValue;
String _luminositeValue;
String _minuteValue;
String _secondeValue;
String _heureValue;
bool SD_Connected = false;
File myFile;

SAVE::SAVE()
{
    _SD_SCLK = 0;
    _SD_MISO = 0;
    _SD_MOSI = 0;
    _SD_CS = 0;
}
bool SAVE::SD_Card_begin(uint8_t SD_SCLK, uint8_t SD_MISO, uint8_t SD_MOSI, uint8_t SD_CS)
{
    _SD_SCLK = SD_SCLK;
    _SD_MISO = SD_MISO;
    _SD_MOSI = SD_MOSI;
    _SD_CS = SD_CS;
    SPI.begin(_SD_SCLK, _SD_MISO, _SD_MOSI, _SD_CS);
    if (SD.begin(SD_CS))
    {
        Serial.println("SD Card Connectée");
        SD_Connected = true;
        uint32_t cardSize = SD.cardSize() / (1024 * 1024);
        String str = "SD Card Size: " + String(cardSize) + "MB";
        SD.remove("/datalog.csv");
        Serial.println(str);
        return true;
    }
    else
    {
        Serial.println("SD Card Déconnectée pas d'enregistrement sur la carte SD");
        return false;
    }
}
void SAVE::get_value_at_save()
{
    _temperatureValue = mySSD1306.returned_temperature();
    _humiditeValue = myAHT20.humidity();
    _luminositeValue = myBH1750.luminosite_directe();
    _minuteValue = myRTC3231.getMinute();
    _heureValue = myRTC3231.getHour();
    _secondeValue = myRTC3231.getSeconds();
}

void SAVE::save_on_SD_card()
{
    if (SD_Connected)
    {
        myFile = SD.open("/datalog.csv", FILE_APPEND);
        if (myFile)
        {
            if (myFile.size() == 0)
            {
                myFile.println("Température,Luminosité,Humidité,Heure:Minute:Seconde");
            }
            myFile.print(_temperatureValue);
            myFile.print(",");
            myFile.print(_luminositeValue);
            myFile.print(",");
            myFile.print(_humiditeValue);
            myFile.print(",");
            myFile.print(_heureValue);
            myFile.print(":");
            myFile.print(_minuteValue);
            myFile.print(":");
            myFile.print(_secondeValue);
            myFile.println();
            myFile.close();
        }
    }
}