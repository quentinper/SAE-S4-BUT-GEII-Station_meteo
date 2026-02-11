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
    drawgaugesTemp();
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
    delay(TEMPO);
    drawgaugesLum();
    delay(TEMPO);
    return;
}

/* ---------------------------------------------------------- */

void SSD1306::affichage_humidite()
{
    float value = myAHT20.humidity();
    oled.clearDisplay();
    oled.drawRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, WHITE);
    oled.drawBitmap(48, 5, epd_bitmap_allArray[ICON_HUMIDITE], 32, 32, WHITE);
    oled.setTextColor(WHITE);
    oled.setTextSize(2);
    oled.setCursor(20, 44);
    oled.print(value);
    oled.print(" %");
    oled.display();
    delay(TEMPO);
    // drawgaugesHum();
    int x2 = (value * 116) / 100;
    oled.clearDisplay();
    oled.drawRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, WHITE);
    oled.drawRect(6, 45, 116, 15, WHITE);
    oled.fillRect(6, 45, x2, 15, WHITE);
    oled.drawBitmap(1, 1, epd_bitmap_allArray[ICON_HUMIDITE], 32, 32, WHITE);
    oled.setTextColor(WHITE);
    oled.setTextSize(2);
    oled.setCursor(40, 16);
    oled.print(value);
    oled.print("%");
    oled.setTextSize(1);
    oled.setCursor(6, 35);
    oled.print("0");
    oled.setCursor(106, 35);
    oled.print("100");
    oled.display();
    delay(TEMPO);
    return;
}

void SSD1306::drawClock()
{
    oled.clearDisplay();
    int h = myRTC3231.getHour() % 12;
    int m = myRTC3231.getMinute();
    int s = myRTC3231.getSeconds();

    float angle_s = s * 6 * PI / 180;
    float angle_m = m * 6 * PI / 180;
    float angle_h = (h * 30 + m * 0.5) * PI / 180;

    int centerX = SCREEN_WIDTH / 2;
    int centerY = SCREEN_HEIGHT / 2;

    // Longueurs adaptées à l'écran
    Position.Px_s = centerX + 25 * cos(angle_s - PI / 2);
    Position.Py_s = centerY + 25 * sin(angle_s - PI / 2);
    Position.Px_m = centerX + 20 * cos(angle_m - PI / 2);
    Position.Py_m = centerY + 20 * sin(angle_m - PI / 2);
    Position.Px_h = centerX + 15 * cos(angle_h - PI / 2);
    Position.Py_h = centerY + 15 * sin(angle_h - PI / 2);

    // Cadran
    for (int i = 0; i < 360; i += 30)
    {
        float rad = i * PI / 180;
        int x1 = centerX + 28 * cos(rad - PI / 2);
        int y1 = centerY + 28 * sin(rad - PI / 2);
        oled.fillCircle(x1, y1, 1, WHITE);
    }

    // Aiguilles
    oled.drawRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, WHITE);
    oled.drawCircle(centerX, centerY, 31, WHITE);
    oled.drawLine(centerX, centerY, Position.Px_h, Position.Py_h, WHITE);
    oled.drawLine(centerX, centerY, Position.Px_m, Position.Py_m, WHITE);
    oled.drawLine(centerX, centerY, Position.Px_s, Position.Py_s, WHITE);
    oled.display();
}

void SSD1306::drawgaugesTemp()
{
    float t = myDS1621.getValue();
    float angle_t = t * 13.84 * PI / 180;
    Position.Px_t = 64 + 25 * cos(angle_t - PI / 2);
    Position.Py_t = 64 + 25 * sin(angle_t - PI / 2);
    oled.clearDisplay();
    for (int i = 0; i < 360; i += 13.84)
    {
        float rad = i * PI / 180;
        int x1 = 64 + 50 * cos(rad - PI / 2);
        int y1 = 64 + 50 * sin(rad - PI / 2);
        oled.fillCircle(x1, y1, 1, WHITE);
    }
    oled.drawRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, WHITE);
    oled.drawLine(64, 64, Position.Px_t, Position.Py_t, WHITE);
    oled.setTextColor(WHITE);
    oled.setTextSize(1);
    oled.setCursor(45, 2);
    oled.print(t);
    oled.setCursor(0, 54);
    oled.print("0");
    oled.setCursor(116, 54);
    oled.print("50");
    oled.drawCircle(64, 64, 50, WHITE);
    oled.display();
}
void SSD1306::drawgaugesLum()
{
    int value = myBH1750.readValue();
    int x2 = (value * 116) / 2000;
    // int y2 = ;
    oled.clearDisplay();
    oled.drawRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, WHITE);
    oled.drawRect(6, 45, 116, 15, WHITE);
    oled.fillRect(6, 45, x2, 15, WHITE);
    oled.drawBitmap(1, 1, epd_bitmap_allArray[ICON_ILLUMINATION], 32, 32, WHITE);
    oled.setTextColor(WHITE);
    oled.setTextSize(2);
    oled.setCursor(40, 16);
    oled.print(myBH1750.readValue());
    oled.setTextSize(1);
    oled.setCursor(6, 35);
    oled.print("0");
    oled.setCursor(100, 35);
    oled.print("2000");
    oled.display();
}
