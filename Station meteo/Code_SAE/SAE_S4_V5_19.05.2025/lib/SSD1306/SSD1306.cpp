/***************************************************************
 * @file        SSD1306.cpp
 * @author      Lorenzo S. & Quentin P.
 * @version     2
 * @date        2025-05-19
 * @brief       Gestion de l'affichage OLED.
 * @details     SAE S4 BUG GEII FA ESE -
 * @copyright   (c) Lorenzo S. & Quentin P.- Tous droits réservés
 ***************************************************************/
#include <SSD1306.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <images.h>

#define MENU_1 0
#define MENU_2 1
#define MENU_3 2
#define MENU_4 3
#define ICON_DATE_HEURE 4
#define DROIT_AUTEUR 5
#define ICON_TEMPERATURE 6
#define ICON_ILLUMINATION 7
#define ICON_SETTINGS 8
#define ICON_WEATHER 9
#define ICON_CALENDAR 10
#define DROIT_CY 11
#define ICON_HUMIDITE 12
#define TEMPO 2000
#define GAUGE_LUM_MAX 2000
#define GAUGE_LUM_MIN 0
#define GAUGE_HUM_MAX 100
#define GAUGE_HUM_MIN 0
#define GAUGE_TEMP_MAX 50
#define GAUGE_TEMP_MIN 0

SSD1306::SSD1306()
{
    _adress = 0;
    _value = 0;
}
//************************************
Adafruit_SSD1306 oled(SCREEN_WIDTH, SCREEN_HEIGHT); // Définition unique
GFXcanvas1 canvas(SCREEN_WIDTH, SCREEN_HEIGHT);
struct
{
    float Py_h = 0;
    float Px_h = 0;
    float Py_s = 0;
    float Px_s = 0;
    float Py_m = 0;
    float Px_m = 0;
    float Px_t = 0;
    float Py_t = 0;

} Position;

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
    GFXcanvas1 canvas(SCREEN_WIDTH, SCREEN_HEIGHT);
    canvas.drawRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, WHITE);
    canvas.drawBitmap(5, 5, epd_bitmap_allArray[ICON_DATE_HEURE], 32, 32, WHITE);
    canvas.setTextColor(WHITE);
    canvas.setTextSize(2);
    canvas.setCursor(50, 5);
    canvas.print(myRTC3231.getStringDay());
    canvas.setCursor(94, 5);
    canvas.print(myRTC3231.getNumber());
    canvas.setCursor(50, 25);
    canvas.print(myRTC3231.getStringMonth());
    canvas.setCursor(20, 45);
    canvas.print(myRTC3231.getStringTime());
    oled.clearDisplay();
    oled.drawBitmap(0, 0, canvas.getBuffer(), SCREEN_WIDTH, SCREEN_HEIGHT, WHITE);
    oled.display();
    delay(TEMPO);
    drawClock();
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
    GFXcanvas1 canvas(SCREEN_WIDTH, SCREEN_HEIGHT);
    float temperature_moyenne = (myDS1621.getValue() + myAHT20.temperature()) / 2;
    String temperature_returned = String((float)temperature_moyenne);
    canvas.drawRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, WHITE);
    canvas.drawBitmap(4, 14, epd_bitmap_allArray[ICON_TEMPERATURE], 32, 32, WHITE);
    canvas.setTextColor(WHITE);
    canvas.setTextSize(2);
    canvas.setCursor(41, 23);
    canvas.print(temperature_moyenne);
    canvas.print((char)247);
    canvas.print("C");
    oled.clearDisplay();
    oled.drawBitmap(0, 0, canvas.getBuffer(), SCREEN_WIDTH, SCREEN_HEIGHT, WHITE);
    oled.display();
    delay(TEMPO);
    drawgaugesTemp();
    delay(TEMPO);
    return temperature_returned;
}

/* ---------------------------------------------------------- */

String SSD1306::returned_temperature()
{
    float temperature_moyenne = (myDS1621.getValue() + myAHT20.temperature()) / 2;
    String temperature_returned = String((float)temperature_moyenne);
    return temperature_returned;
}

/* ---------------------------------------------------------- */

void SSD1306::affichage_droit_auteur()
{
    GFXcanvas1 canvas(SCREEN_WIDTH, SCREEN_HEIGHT);
    canvas.setTextColor(WHITE);
    canvas.setTextSize(2);
    canvas.drawBitmap(0, 0, epd_bitmap_allArray[DROIT_CY], 128, 64, WHITE);
    oled.clearDisplay();
    oled.drawBitmap(0, 0, canvas.getBuffer(), SCREEN_WIDTH, SCREEN_HEIGHT, WHITE);
    oled.display();
    delay(5000);
    canvas.fillScreen(BLACK);
    canvas.drawBitmap(48, 0, epd_bitmap_allArray[DROIT_AUTEUR], 32, 32, WHITE);
    canvas.setCursor(10, 30);
    canvas.print("Lorenzo.S");
    canvas.setCursor(10, 45);
    canvas.print("Quentin.P");
    oled.clearDisplay();
    oled.drawBitmap(0, 0, canvas.getBuffer(), SCREEN_WIDTH, SCREEN_HEIGHT, WHITE);
    oled.display();
    delay(2000);
    return;
}

/* ---------------------------------------------------------- */

void SSD1306::affichage_luminosite()
{
    GFXcanvas1 canvas(SCREEN_WIDTH, SCREEN_HEIGHT);
    int value = myBH1750.readValue();
    canvas.drawRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, WHITE);
    canvas.drawBitmap(48, 0, epd_bitmap_allArray[ICON_ILLUMINATION], 32, 32, WHITE);
    canvas.setTextColor(WHITE);
    canvas.setTextSize(2);
    canvas.setCursor(10, 44);
    canvas.print(value);
    canvas.print(" Lx");
    oled.clearDisplay();
    oled.drawBitmap(0, 0, canvas.getBuffer(), SCREEN_WIDTH, SCREEN_HEIGHT, WHITE);
    oled.display();
    delay(TEMPO);
    drawgaugesLum();
    delay(TEMPO);
    return;
}

/* ---------------------------------------------------------- */

void SSD1306::affichage_humidite()
{
    float value = myAHT20.humidity();
    GFXcanvas1 canvas(SCREEN_WIDTH, SCREEN_HEIGHT);
    canvas.drawRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, WHITE);
    canvas.drawBitmap(48, 5, epd_bitmap_allArray[ICON_HUMIDITE], 32, 32, WHITE);
    canvas.setTextColor(WHITE);
    canvas.setTextSize(2);
    canvas.setCursor(20, 44);
    canvas.print(value);
    canvas.print(" %");
    oled.clearDisplay();
    oled.drawBitmap(0, 0, canvas.getBuffer(), SCREEN_WIDTH, SCREEN_HEIGHT, WHITE);
    oled.display();
    delay(TEMPO);
    canvas.fillScreen(BLACK);
    int x2 = (value * 116) / 100;
    canvas.drawRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, WHITE);
    canvas.drawRect(6, 45, 116, 15, WHITE);
    canvas.fillRect(6, 45, x2, 15, WHITE);
    canvas.drawBitmap(1, 1, epd_bitmap_allArray[ICON_HUMIDITE], 32, 32, WHITE);
    canvas.setTextColor(WHITE);
    canvas.setTextSize(2);
    canvas.setCursor(40, 16);
    canvas.print(value);
    canvas.print("%");
    canvas.setTextSize(1);
    canvas.setCursor(6, 35);
    canvas.print("0");
    canvas.setCursor(106, 35);
    canvas.print("100");
    oled.clearDisplay();
    oled.drawBitmap(0, 0, canvas.getBuffer(), SCREEN_WIDTH, SCREEN_HEIGHT, WHITE);
    oled.display();
    delay(TEMPO);
    return;
}

/* ---------------------------------------------------------- */

void SSD1306::drawClock()
{
    GFXcanvas1 canvas(SCREEN_WIDTH, SCREEN_HEIGHT);
    int h = myRTC3231.getHour() % 12;
    int m = myRTC3231.getMinute();
    int s = myRTC3231.getSeconds();
    float angle_s = s * 6 * PI / 180;
    float angle_m = m * 6 * PI / 180;
    float angle_h = (h * 30 + m * 0.5) * PI / 180;
    int centerX = SCREEN_WIDTH / 2;
    int centerY = SCREEN_HEIGHT / 2;
    Position.Px_s = centerX + 25 * cos(angle_s - PI / 2);
    Position.Py_s = centerY + 25 * sin(angle_s - PI / 2);
    Position.Px_m = centerX + 20 * cos(angle_m - PI / 2);
    Position.Py_m = centerY + 20 * sin(angle_m - PI / 2);
    Position.Px_h = centerX + 15 * cos(angle_h - PI / 2);
    Position.Py_h = centerY + 15 * sin(angle_h - PI / 2);
    for (int i = 0; i < 360; i += 30)
    {
        float rad = i * PI / 180;
        int x1 = centerX + 28 * cos(rad - PI / 2);
        int y1 = centerY + 28 * sin(rad - PI / 2);
        canvas.fillCircle(x1, y1, 1, WHITE);
    }
    canvas.drawRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, WHITE);
    canvas.drawCircle(centerX, centerY, 31, WHITE);
    canvas.drawLine(centerX, centerY, Position.Px_h, Position.Py_h, WHITE);
    canvas.drawLine(centerX, centerY, Position.Px_m, Position.Py_m, WHITE);
    canvas.drawLine(centerX, centerY, Position.Px_s, Position.Py_s, WHITE);
    oled.clearDisplay();
    oled.drawBitmap(0, 0, canvas.getBuffer(), SCREEN_WIDTH, SCREEN_HEIGHT, WHITE);
    oled.display();
}

/* ---------------------------------------------------------- */

void SSD1306::drawgaugesTemp()
{
    GFXcanvas1 canvas(SCREEN_WIDTH, SCREEN_HEIGHT);
    float t = myDS1621.getValue();
    float angle_t = t * 13.84 * PI / 180;
    Position.Px_t = 64 + 25 * cos(angle_t - PI / 2);
    Position.Py_t = 64 + 25 * sin(angle_t - PI / 2);

    for (int i = 0; i < 360; i += 13.84)
    {
        float rad = i * PI / 180;
        int x1 = 64 + 50 * cos(rad - PI / 2);
        int y1 = 64 + 50 * sin(rad - PI / 2);
        canvas.fillCircle(x1, y1, 1, WHITE);
    }
    canvas.drawRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, WHITE);
    canvas.drawLine(64, 64, Position.Px_t, Position.Py_t, WHITE);
    canvas.setTextColor(WHITE);
    canvas.setTextSize(1);
    canvas.setCursor(45, 2);
    canvas.print(t);
    canvas.setCursor(0, 54);
    canvas.print(GAUGE_TEMP_MIN);
    canvas.setCursor(116, 54);
    canvas.print(GAUGE_TEMP_MAX);
    canvas.drawCircle(64, 64, 50, WHITE);
    oled.clearDisplay();
    oled.drawBitmap(0, 0, canvas.getBuffer(), SCREEN_WIDTH, SCREEN_HEIGHT, WHITE);
    oled.display();
}

/* ---------------------------------------------------------- */

void SSD1306::drawgaugesLum()
{
    GFXcanvas1 canvas(SCREEN_WIDTH, SCREEN_HEIGHT);
    int value = myBH1750.readValue();
    int x2 = (value * 116) / GAUGE_LUM_MAX;
    canvas.drawRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, WHITE);
    canvas.drawRect(6, 45, 116, 15, WHITE);
    canvas.fillRect(6, 45, x2, 15, WHITE);
    canvas.drawBitmap(1, 1, epd_bitmap_allArray[ICON_ILLUMINATION], 32, 32, WHITE);
    canvas.setTextColor(WHITE);
    canvas.setTextSize(2);
    canvas.setCursor(40, 16);
    canvas.print(value);
    canvas.setTextSize(1);
    canvas.setCursor(6, 35);
    canvas.print(GAUGE_LUM_MIN);
    canvas.setCursor(100, 35);
    canvas.print(GAUGE_LUM_MAX);
    oled.clearDisplay();
    oled.drawBitmap(0, 0, canvas.getBuffer(), SCREEN_WIDTH, SCREEN_HEIGHT, WHITE);
    oled.display();
}
