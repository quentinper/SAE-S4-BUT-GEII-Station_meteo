#ifndef RTC_3231_H
#define RTC_3231_H
#include <Arduino.h>
#include <Wire.h>

// les registres
#define RTC3231_seconds 0x00
#define RTC3231_minutes 0x01
#define RTC3231_hours 0x02
#define RTC3231_day 0x03
#define RTC3231_date 0x04
#define RTC3231_month 0x05
#define RTC3231_year 0x06

class RTC3231
{
public:
    // constructeur
    RTC3231();
    // les méthodes
    bool begin(uint8_t RTC3231_adr);
    void setTime(uint8_t hh, uint8_t mm, uint8_t ss);
    void setDate(uint8_t index_day, uint8_t num, uint8_t index_month, uint16_t year);
    uint8_t getHour();       // hh
    uint8_t getMinute();     // mm
    uint8_t getSeconds();    // ss
    String getStringDay();   // Dim, lun, Mar, Mer, Jeu, Ven,
    String getStringMonth(); // janv fev mars avril
    uint8_t getNumber();
    uint16_t getYear();     // 1 to 31
    String getStringTime(); // hh:mm
    String getStringDate(); // Day number month
    String getAllData();

private:
    uint8_t _adress;
    uint8_t _binToBcd(uint8_t val);
    uint8_t _bcdToBin(uint8_t val);
};

#endif

// dans le setup
