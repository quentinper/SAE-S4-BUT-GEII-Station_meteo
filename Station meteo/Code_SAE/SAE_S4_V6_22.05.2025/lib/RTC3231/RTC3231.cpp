/***************************************************************
 * @file        RTC3231.cpp
 * @author      Lorenzo S. & Quentin P.
 * @version     2
 * @date        2025-05-19
 * @brief       Gestion de la real time clock
 * @details     SAE S4 BUG GEII FA ESE
 * @copyright   (c) Lorenzo S. & Quentin P.- Tous droits réservés
 ***************************************************************/
#include <RTC3231.h>

const char *days_week[] = {"Dim", "Lun", "Mar", "Mer", "Jeu", "Ven"};
const char *months_year[] = {"Jan", "Fev", "Mar", "Avril", "Mai", "Jui", "Juil", "Aou", "Sep", "Oct", "Nov", "Dec"};

RTC3231::RTC3231()
{
    _adress = 0;
}

/* ---------------------------------------------------------- */

bool RTC3231::begin(uint8_t RTC3231_adr)
{
    _adress = RTC3231_adr;

    Wire.begin();
    Wire.beginTransmission(_adress);
    if (Wire.endTransmission() == 0)
    {
        Serial.println("Capteur DS3231 connecté.");
        return true;
    }
    else
    {
        Serial.println("Échec de connexion au capteur DS3231 !");
        return false;
    }
}

/* ---------------------------------------------------------- */

uint8_t RTC3231::_binToBcd(uint8_t val)
{
    uint8_t A = val / 10;
    A = A * 16;
    uint8_t B = val % 10;
    uint8_t S = A + B;
    return S;
}

/* ---------------------------------------------------------- */

uint8_t RTC3231::_bcdToBin(uint8_t val)
{
    uint8_t A = val / 16;
    A = A * 10;
    uint8_t B = val % 16;
    uint8_t S = A + B;
    return S;
}

/* ---------------------------------------------------------- */

uint8_t RTC3231::getSeconds()
{
    uint8_t seconds = 0;
    Wire.beginTransmission(_adress);
    Wire.write(RTC3231_seconds);
    Wire.endTransmission();

    Wire.requestFrom(_adress, byte(1));
    seconds = Wire.read();
    Wire.endTransmission();
    seconds = seconds & 0x7F;
    seconds = _bcdToBin(seconds);
    return seconds;
}

/* ---------------------------------------------------------- */

uint8_t RTC3231::getHour()
{
    uint8_t hours = 0;
    Wire.beginTransmission(_adress);
    Wire.write(RTC3231_hours);
    Wire.endTransmission();
    Wire.requestFrom(_adress, byte(1));
    hours = Wire.read();
    Wire.endTransmission();
    hours = hours & 0x7F;
    hours = _bcdToBin(hours);
    return hours;
}

/* ---------------------------------------------------------- */

uint8_t RTC3231::getMinute()
{
    uint8_t Minutes = 0;
    //
    Wire.beginTransmission(_adress);
    Wire.write(RTC3231_minutes);
    Wire.endTransmission();

    Wire.requestFrom(_adress, byte(1));
    Minutes = Wire.read();
    Wire.endTransmission();
    Minutes = Minutes & 0x7F;
    Minutes = _bcdToBin(Minutes);
    return Minutes;
}

/* ---------------------------------------------------------- */

String RTC3231::getStringDay()
{
    int day_ = 0;
    Wire.beginTransmission(_adress);
    Wire.write(RTC3231_day);
    Wire.endTransmission();
    Wire.requestFrom(_adress, byte(1));
    day_ = Wire.read();
    Wire.endTransmission();
    String day = days_week[day_];
    return day;
}

/* ---------------------------------------------------------- */

String RTC3231::getStringMonth()
{
    int Months_ = 0;
    Wire.beginTransmission(_adress);
    Wire.write(RTC3231_month);
    Wire.endTransmission();
    Wire.requestFrom(_adress, byte(1));
    Months_ = Wire.read();
    Wire.endTransmission();
    String Months = months_year[Months_];
    return Months;
}

/* ---------------------------------------------------------- */

uint8_t RTC3231::getNumber()
{
    uint8_t Number = 0;
    Wire.beginTransmission(_adress);
    Wire.write(RTC3231_date);
    Wire.endTransmission();
    Wire.requestFrom(_adress, byte(1));
    Number = Wire.read();
    Wire.endTransmission();
    Number = Number & 0x7F;
    Number = _bcdToBin(Number);
    return Number;
}

/* ---------------------------------------------------------- */

uint16_t RTC3231::getYear()
{
    Wire.beginTransmission(_adress);
    Wire.write(RTC3231_year); // Adresse du registre de l'année
    Wire.endTransmission();
    Wire.requestFrom(_adress, byte(1));
    Wire.endTransmission();
    uint8_t year = Wire.read();    // Lire l'année en BCD
    return _bcdToBin(year) + 2000; // Convertir et ajouter 2000
}

/* ---------------------------------------------------------- */

String RTC3231::getStringTime()
{

    uint8_t hours = getHour();
    uint8_t minutes = getMinute();
    uint8_t seconds = getSeconds();

    String hoursStr = String(hours);
    String minutesStr = String(minutes);
    String secondsStr = String(seconds);

    if (hours < 10)
        hoursStr = "0" + hoursStr; // Ajoute un zéro devant l'heure si < 10
    if (minutes < 10)
        minutesStr = "0" + minutesStr; // Ajoute un zéro devant les minutes si < 10
    if (seconds < 10)
        secondsStr = "0" + secondsStr; // Ajoute un zéro devant les secondes si < 10

    String Heures_assemblee = hoursStr + ":" + minutesStr + ":" + secondsStr;
    return Heures_assemblee;
}

/* ---------------------------------------------------------- */

String RTC3231::getStringDate()
{
    String Number = String((uint8_t)getNumber());
    String Day = getStringDay();
    String Month = getStringMonth();
    String Date_complete = Day + " " + Number + " " + Month;
    return Date_complete;
}

/* ---------------------------------------------------------- */

String RTC3231::getAllData()
{
    String Heures_ = getStringTime();
    String Dates_ = getStringDate();
    String Annee_ = String((uint16_t)getYear());
    String allData = Dates_ + " " + Annee_ + " " + Heures_;
    Serial.println(allData);
    return allData;
}

/* ---------------------------------------------------------- */

void RTC3231::setTime(uint8_t hh, uint8_t mm, uint8_t ss)
{
    Wire.beginTransmission(_adress);
    Wire.write(0x00); // Adresse du registre de l'heure
    Wire.write(_binToBcd(ss));
    Wire.write(_binToBcd(mm));
    Wire.write(_binToBcd(hh));
    Wire.endTransmission();
}

/* ---------------------------------------------------------- */

void RTC3231::setDate(uint8_t index_day, uint8_t num, uint8_t index_month, uint16_t year)
{
    Wire.beginTransmission(_adress);
    Wire.write(RTC3231_day);            // Adresse du registre du jour de la semaine
    Wire.write(_binToBcd(index_day));   // Jour de la semaine (1 = Lundi, ..., 7 = Dimanche)
    Wire.write(_binToBcd(num));         // Jour du mois (1-31)
    Wire.write(_binToBcd(index_month)); // Mois (1-12)
    Wire.write(_binToBcd(year - 2000)); // Année (0-99)
    Wire.endTransmission();
}

/* ---------------------------------------------------------- */
