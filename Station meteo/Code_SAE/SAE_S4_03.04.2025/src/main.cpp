
#include <Ticker.h>
#include <DS1621.h>
#include <RTC3231.h>
#include <SSD1306.h>
#include <BH1750.h>
#include <AHT20.h>

/* ----------------------constant------------------------------ */

#define DS1621_adr 0x49
#define RTC3231_adr 0x68
#define SSD1306_adr 0x3C
#define BH1750_adr 0x23
#define AHT20_adr 0x38

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define BUTTON_UP_PIN 15
#define BUTTON_SELECT_PIN 12
#define BUTTON_DOWN_PIN 14

const uint32_t PERIOD = 1000;
const int maxIndex = 2;
float temp;
float lux = 0;
int i = 0, value = 0;

/* ----------------------Horloge------------------------------ */

uint8_t hh = 1;         /* Heure */
uint8_t mm = 1;         /* Minute */
uint8_t ss = 1;         /* Seconde */
uint8_t index_day = 2;   /* Jour de la semaine (0 = dimanche, 1 = lundi, etc.) */
uint8_t num = 1;        /* Jour du mois */
uint8_t index_month = 3; /* Mois (1 = janvier, 2 = février, etc.) */
uint16_t year = 2025;    /* Année */

/* ---------------------------------------------------------- */

/* -------------------------Obejt--------------------------------- */

DS1621 myDS1621;
RTC3231 myRTC3231;
SSD1306 mySSD1306;
BH1750 myBH1750;
AHT20 myAHT20;

/* -------------------------Prototypes--------------------------------- */

void action();
void Affichage_ecran();
Ticker myTicker(action, PERIOD, 0, MILLIS);
int affichage_heures();
int affichage_temperature();
int affichage_parametre();
void affichage_droit_auteur();

/*--------------------action-------------------------------------- */

void action()
{
  Affichage_ecran();
  bool res20 = myAHT20.getValues();
}

/* -------------------------Affichage_ecran--------------------------------- */

void Affichage_ecran()
{
  // Affichage du bitmap correspondant à l'index i
  if (value != 1 && value != 2 && value != 3)
  {
    mySSD1306.affichage_menus(i);
  }
  switch (value)
  {
  case 1:
    mySSD1306.affichage_heures();
    value = 0;
    break;

  case 2:
    mySSD1306.affichage_temperature();
    value = 0;
    break;

  case 3:
    mySSD1306.affichage_luminosite();
    value = 0;
    break;

  default:
    break;
  }
}

/* -----------------------------commande_utilisateur----------------------------- */

void commande_utilisateur()
{
  if (digitalRead(BUTTON_UP_PIN) == HIGH)
  {
    i++;
    if (i > maxIndex)
      i = 0;
    delay(200); // Anti-rebond
  }
  else if (digitalRead(BUTTON_DOWN_PIN) == HIGH)
  {
    i--;
    if (i < 0)
      i = maxIndex;
    delay(200); // Anti-rebond
  }
  else if (digitalRead(BUTTON_SELECT_PIN) == HIGH)
  {
    if (i == 0)
    {
      value = 1;
      delay(200);
    }
    else if (i == 1)
    {
      value = 2;
      delay(200);
    }
    else if (i == 2)
    {
      value = 3;
      delay(200);
    }
  }
}

/* ----------------------------setup------------------------------ */

void setup()
{
  Serial.begin(115200);
  Wire.begin();
  //
  mySSD1306.beginOLED(SSD1306_adr);
  bool state_DS1621 = myDS1621.begin(DS1621_adr);
  if (state_DS1621)
  {
    myDS1621.startConv();
  }
  bool state_RTC3231 = myRTC3231.begin(RTC3231_adr);
  if (state_RTC3231)
  {
    myRTC3231.setTime(hh, mm, ss);
    myRTC3231.setDate(index_day, num, index_month, year);
  }
  bool state_BH1750 = myBH1750.begin(BH1750_adr);
  if (state_BH1750)
  {
    myBH1750.begin(BH1750_adr); // Double begin ?
    myBH1750.powerOn();
    myBH1750.setMode(0X10);
  }

  /* *LORENZO* */
  bool state_AHT20 = myAHT20.begin(AHT20_adr);
  if (state_AHT20)
  {
    // a finir ...
  }

  mySSD1306.affichage_droit_auteur();
  myTicker.start();
}

/* ---------------------------loop------------------------------- */

void loop()
{
  myTicker.update();      // Met à jour le Ticker
  commande_utilisateur(); // gestion commande utilisateur
}

/* ---------------------------------------------------------- */