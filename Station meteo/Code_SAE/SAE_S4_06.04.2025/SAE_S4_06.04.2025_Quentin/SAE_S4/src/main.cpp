
#include <Ticker.h>
#include <DS1621.h>
#include <RTC3231.h>
#include <SSD1306.h>
#include <BH1750.h>
#include <AHT20.h>
#include <WiFi.h>
#include <LittleFS.h>
#include <ESPAsyncWebServer.h>

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
const int maxIndex = 3;
float temp;
float lux = 0;
int i = 0, value = 0;
const char *ssid = "Linksys01370";
const char *password = "3fanq5w4pb";
String temperatureValue;
/* ----------------------Horloge------------------------------ */

uint8_t hh = 1;          /* Heure */
uint8_t mm = 1;          /* Minute */
uint8_t ss = 1;          /* Seconde */
uint8_t index_day = 2;   /* Jour de la semaine (0 = dimanche, 1 = lundi, etc.) */
uint8_t num = 1;         /* Jour du mois */
uint8_t index_month = 3; /* Mois (1 = janvier, 2 = février, etc.) */
uint16_t year = 2025;    /* Année */

/* ---------------------------------------------------------- */

/* -------------------------Obejt--------------------------------- */

DS1621 myDS1621;
RTC3231 myRTC3231;
SSD1306 mySSD1306;
BH1750 myBH1750;
AHT20 myAHT20;
//
AsyncWebServer server(80);

/* -------------------------Prototypes--------------------------------- */

void action();
void Affichage_ecran();
Ticker myTicker(action, PERIOD, 0, MILLIS);
int affichage_heures();
int affichage_temperature();
int affichage_parametre();
void affichage_droit_auteur();
void function_connect(WiFiEvent_t event, WiFiEventInfo_t info);
void function_disconnect(WiFiEvent_t event, WiFiEventInfo_t info);
void function_got_ip(WiFiEvent_t event, WiFiEventInfo_t info);
void set_routes();

/*--------------------WIFI-------------------------------------- */

void initLittleFS()
{
  if (LittleFS.begin(true) == false)
  {
    Serial.println("Error Init LittleFs");
  }
  else
  {
    File root = LittleFS.open("/");
    File file = root.openNextFile();
    while (file)
    {
      Serial.print("File: ");
      Serial.println(file.name());
      file.close();
      file = root.openNextFile();
    }
  }
}

void function_connect(WiFiEvent_t event, WiFiEventInfo_t info)
{
}

void function_disconnect(WiFiEvent_t event, WiFiEventInfo_t info)
{
}

void function_got_ip(WiFiEvent_t event, WiFiEventInfo_t info)
{
  Serial.print("Adresse Ip : ");
  Serial.println(WiFi.localIP());
  digitalWrite(LED_BUILTIN, HIGH);
  //
  server.begin();
  Serial.print("Serveur started on port 80");
}

/*--------------------action-------------------------------------- */

void action()
{
  Affichage_ecran();
  temperatureValue = mySSD1306.affichage_temperature();
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
  case 4:
    mySSD1306.affichage_humidite();
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
    else if (i == 3)
    {
      value = 4;
      delay(200);
    }
  }
}

void set_routes()
{

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(LittleFS, "/index.html", "text/html"); });
  //
  server.on("/w3.css", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(LittleFS, "/w3.css", "text/css"); });
  //
  server.on("/script.js", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(LittleFS, "/script.js", "text/javascript"); });

  server.on("/gauge.min.js", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(LittleFS, "/gauge.min.js", "text/javascript"); });

  server.on("/action", HTTP_GET, [](AsyncWebServerRequest *request)
            {
            if(request->hasParam("led"))
            {
              String res=request->getParam("led")->value();
              Serial.println(res);
              if(res == "1")
              digitalWrite(21, HIGH);
              if(res == "0")
              digitalWrite(21, LOW);
            }
              request->send(200); });
  //
  server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(200, "/text/plain", String(temperatureValue)); });
  //
}
/* ----------------------------setup------------------------------ */

void setup()
{
  Serial.begin(115200);
  pinMode(21, OUTPUT);
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
    myBH1750.powerOn();
    myBH1750.setMode(0X10);
  }
  bool state_AHT20 = myAHT20.begin(AHT20_adr);
  if (state_AHT20)
  {
    // NA  ?
  }
  pinMode(LED_BUILTIN, OUTPUT);
  WiFi.begin(ssid, password);
  WiFi.mode(WIFI_STA);
  WiFi.onEvent(function_connect, ARDUINO_EVENT_WIFI_STA_CONNECTED);
  WiFi.onEvent(function_disconnect, ARDUINO_EVENT_WIFI_STA_DISCONNECTED);
  WiFi.onEvent(function_got_ip, ARDUINO_EVENT_WIFI_STA_GOT_IP);
  initLittleFS();
  mySSD1306.affichage_droit_auteur();
  //
  set_routes();
  myTicker.start();
}

/* ---------------------------loop------------------------------- */

void loop()
{
  myTicker.update();      // Met à jour le Ticker
  commande_utilisateur(); // gestion commande utilisateur
}

/* ---------------------------------------------------------- */