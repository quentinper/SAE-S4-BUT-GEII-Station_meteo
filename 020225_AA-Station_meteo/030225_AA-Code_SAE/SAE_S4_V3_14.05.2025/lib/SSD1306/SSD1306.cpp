#include <SSD1306.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <images.h>

#define MENU_1 0
#define MENU_2 1
#define MENU_3 2
#define ICON_DATE_HEURE 3
#define DROIT_AUTEUR 4
#define ICON_TEMPERATURE 5
#define ICON_ILLUMINATION 6
#define ICON_SETTINGS 7
#define ICON_WEATHER 8
#define ICON_CALENDAR 9
#define DROIT_CY 10
#define ICON_HUMIDITE 11
#define TEMPO 10000

SSD1306::SSD1306()
{
    _adress = 0;
    _value = 0;
}
//************************************
Adafruit_SSD1306 oled(SCREEN_WIDTH, SCREEN_HEIGHT); // Définition unique

/* ---------------------------------------------------------- */

void SSD1306::beginOLED(uint8_t SSD1306_adr)
{
    _adress = SSD1306_adr;
    if (!oled.begin(SSD1306_SWITCHCAPVCC, _adress))
    {
        Serial.println(F("Échec de l'initialisation de l'écran OLED"));
        while (true)
            ;
    }
    else
    {
        Serial.println("Capteur SSD1306 connecté");
    }
    oled.clearDisplay();
    oled.display();
}
/* ---------------------------------------------------------- */

void SSD1306::affichage_heures()
{
    oled.clearDisplay();
    oled.drawRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, WHITE);
    oled.drawBitmap(5, 5, epd_bitmap_allArray[ICON_DATE_HEURE], 32, 32, WHITE);
    oled.setTextColor(WHITE);
    oled.setTextSize(2);
    oled.setCursor(50, 5);
    oled.print(myRTC3231.getStringDay());
    oled.setCursor(94, 5);
    oled.print(myRTC3231.getNumber());
    oled.setCursor(50, 25);
    oled.print(myRTC3231.getStringMonth());
    oled.setCursor(20, 45);
    oled.print(myRTC3231.getStringTime());
    oled.display();
    delay(TEMPO);
    return;
}
/* ---------------------------------------------------------- */

void SSD1306::affichage_menus(int i)
{
    oled.clearDisplay();
    oled.drawBitmap(0, 0, epd_bitmap_allArray[i], 128, 64, WHITE);
    oled.display();
}

/* ---------------------------------------------------------- */

String SSD1306::affichage_temperature()
{
    oled.clearDisplay();
    oled.drawRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, WHITE);
    oled.drawBitmap(4, 14, epd_bitmap_allArray[ICON_TEMPERATURE], 32, 32, WHITE);
    oled.setTextColor(WHITE);
    oled.setTextSize(2);
    oled.setCursor(41, 23);
    float temperature_moyenne = (myDS1621.getValue() + myAHT20.temperature()) / 2;
    String temperature_returned = String((float)temperature_moyenne);
    oled.print(temperature_moyenne);
    oled.print((char)247);
    oled.print("C");
    oled.display();
    delay(TEMPO);
    return temperature_returned;
}

String SSD1306::returned_temperature()
{
    float temperature_moyenne = (myDS1621.getValue() + myAHT20.temperature()) / 2;
    String temperature_returned = String((float)temperature_moyenne);
    return temperature_returned;
}

/* ---------------------------------------------------------- */

void SSD1306::affichage_droit_auteur()
{
    oled.clearDisplay();
    oled.setTextColor(WHITE);
    oled.setTextSize(2);
    oled.drawBitmap(0, 0, epd_bitmap_allArray[DROIT_CY], 128, 64, WHITE);
    oled.display();
    delay(5000);
    oled.clearDisplay();
    oled.drawBitmap(48, 0, epd_bitmap_allArray[DROIT_AUTEUR], 32, 32, WHITE);
    oled.setCursor(10, 30);
    oled.print("Lorenzo.S");
    oled.setCursor(10, 45);
    oled.print("Quentin.P");
    oled.display();
    delay(2000);
    return;
}

/* ---------------------------------------------------------- */

void SSD1306::affichage_luminosite()
{
    oled.clearDisplay();
    oled.drawRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, WHITE);
    oled.drawBitmap(48, 0, epd_bitmap_allArray[ICON_ILLUMINATION], 32, 32, WHITE);
    oled.setTextColor(WHITE);
    oled.setTextSize(2);
    oled.setCursor(10, 44);
    oled.print(myBH1750.readValue());
    oled.print(" Lx");
    oled.display();
    delay(10000);
    return;
}

/* ---------------------------------------------------------- */

void SSD1306::affichage_humidite()
{
    oled.clearDisplay();
    oled.drawRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, WHITE);
    oled.drawBitmap(48, 0, epd_bitmap_allArray[ICON_HUMIDITE], 32, 32, WHITE);
    oled.setTextColor(WHITE);
    oled.setTextSize(2);
    oled.setCursor(10, 44);
    oled.print(myAHT20.humidity());
    oled.print(" %");
    oled.display();
    delay(10000);
    return;
}