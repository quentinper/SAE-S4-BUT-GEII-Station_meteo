/***************************************************************
 * @file        main.cpp
 * @author      Lorenzo S. & Quentin P.
 * @version     2.0
 * @date        2025-05-19
 * @brief       Point d'entrée principal du programme.
 * @details     Initialise les capteurs, l'afficheur SSD1306,
 *              et lance les différentes fonctions d'affichage
 *              (heure, température, humidité, luminosité, etc.).
 * @copyright   (c) Lorenzo S. & Quentin P. - Tous droits réservés
 ***************************************************************/
#include <LIB.h>
#include <OBJ.h>
/* ----------------------Constant------------------------------ */
//-------------------------@I2C--------------------------------//
#define DS1621_ADR 0x49
#define RTC3231_ADR 0x68
#define SSD1306_ADR 0x3C
#define BH1750_ADR 0x23
#define AHT20_ADR 0x38
//-----------Screen Setup-----------//
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
//-----------PIN Configuration-----------//
#define BUTTON_UP_PIN 15
#define BUTTON_SELECT_PIN 12
#define BUTTON_DOWN_PIN 14
#define SD_MOSI 18
#define SD_MISO 19
#define SD_SCLK 5
#define SD_CS 4
//-----------Tempo-----------//
#define PERIOD 1000
//-----------Variables-----------//
const char *ssid = "Bbox-22C6CF48";
const char *password = "wZp1bYg!sS1hx@dk";
// const char *ssid = "Linksys01370";
// const char *password = "3fanq5w4pb";
/* -------------------------Obejt--------------------------------- */
DS1621 myDS1621;     // Capteur de Température
RTC3231 myRTC3231;   // Real time clock
SSD1306 mySSD1306;   // Afficheur Oled
BH1750 myBH1750;     // Capteur de luminosité
AHT20 myAHT20;       // Capteur d'humidité et de température
CONFWIFI myCONFWIFI; // Configuration du WIFI
SAVE mySAVE;         // Configuration de la carte SD
CMDUSER myCMDUSER;   // Gestion des commandes utilisateurs
/* -------------------------Prototypes--------------------------------- */
void action();
void setup();
void loop();
Ticker myTicker(action, PERIOD, 0, MILLIS);
/*--------------------action-------------------------------------- */
void action()
{
  mySAVE.get_value_at_save();
  myCMDUSER.affichage_ecran();
  myCONFWIFI.get_value();
  mySAVE.save_on_SD_card();
}
/* ----------------------------setup------------------------------ */
void setup()
{
  Serial.begin(115200);
  pinMode(21, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  Wire.begin();
  mySSD1306.beginOLED(SSD1306_ADR);
  bool state_DS1621 = myDS1621.begin(DS1621_ADR);
  if (state_DS1621)
  {
    myDS1621.startConv();
  }
  bool state_BH1750 = myBH1750.begin(BH1750_ADR);
  if (state_BH1750)
  {
    myBH1750.powerOn();
    myBH1750.setMode(0X10);
  }
  bool state_RTC3231 = myRTC3231.begin(RTC3231_ADR);
  bool state_AHT20 = myAHT20.begin(AHT20_ADR);
  bool state_SDCard = mySAVE.SD_Card_begin(SD_SCLK, SD_MISO, SD_MOSI, SD_CS);
  bool state_WIFI = myCONFWIFI.wifi_begin(ssid, password);
  myCONFWIFI.get_state(state_DS1621, state_RTC3231, state_BH1750, state_AHT20, state_SDCard, state_WIFI);
  myCONFWIFI.set_routes();
  mySSD1306.affichage_droit_auteur();
  myTicker.start();
}
/* ---------------------------loop------------------------------- */
void loop()
{
  myTicker.update();
  myCMDUSER.commande_utilisateur(BUTTON_UP_PIN, BUTTON_SELECT_PIN, BUTTON_DOWN_PIN);
}
