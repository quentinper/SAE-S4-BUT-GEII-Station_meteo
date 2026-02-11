#include <Arduino.h>
#include <Wire.h>
#include <CONFWIFI.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <LittleFS.h>
//
AsyncWebServer server(80);
String temperatureValue;
String humiditeValue;
String luminositeValue;
String minuteValue;
String secondeValue;
String heureValue;
uint8_t ss = 0;
bool _StateAHT20;
bool _StateBH1750;
bool _StateWIFI;
bool _StateDS1621;
bool _StateRTC3231;
bool _StateSAVE;
bool _StateSSD1306;
const String API_KEY = "1b29e021e40585dc59333054775832a4";
const String URL = "http://api.openweathermap.org/data/2.5/weather?q=";
const String UNITS = "&units=metric";
const String LANG = "&lang=fr";
String zeCity = "";
float zeTemperature = 0;
float zePressure = 0;
float zeHumidity = 0;

CONFWIFI::CONFWIFI()
{
  _ssid = "0";
  _password = "0";
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

bool CONFWIFI::wifi_begin(const char *ssid, const char *password)
{
  _ssid = ssid;
  _password = password;
  WiFi.begin(_ssid, _password);
  WiFi.mode(WIFI_STA);
  WiFi.onEvent(function_connect, ARDUINO_EVENT_WIFI_STA_CONNECTED);
  WiFi.onEvent(function_disconnect, ARDUINO_EVENT_WIFI_STA_DISCONNECTED);
  WiFi.onEvent(function_got_ip, ARDUINO_EVENT_WIFI_STA_GOT_IP);
  initLittleFS();
  return true;
}

void CONFWIFI::initLittleFS()
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

void CONFWIFI::get_value()
{
  temperatureValue = mySSD1306.returned_temperature();
  humiditeValue = myAHT20.humidity();
  luminositeValue = myBH1750.luminosite_directe();
  minuteValue = myRTC3231.getMinute();
  heureValue = myRTC3231.getHour();
  secondeValue = myRTC3231.getSeconds();
}
void CONFWIFI::get_state(bool state_DS1621, bool state_RTC3231, bool state_BH1750, bool state_AHT20, bool state_SDCard, bool state_WIFI)
{
  _StateAHT20 = state_AHT20;
  _StateBH1750 = state_BH1750;
  _StateDS1621 = state_DS1621;
  _StateRTC3231 = state_RTC3231;
  _StateSAVE = state_SDCard;
  _StateWIFI = state_WIFI;
}

void CONFWIFI::set_routes()
{
  get_value();
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
              if(res == "1"){
                analogWrite(21, 100);}
              if(res == "0"){
                analogWrite(21, LOW);}
            }
              request->send(200); });
  //
  server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(200, "/text/plain", String(temperatureValue)); });

  server.on("/humidity", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(200, "/text/plain", String(humiditeValue)); });
  //
  server.on("/luminosite", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(200, "/text/plain", String(luminositeValue)); });

  server.on("/courbetemp", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(200, "/text/plain", String(temperatureValue)); });

  server.on("/courbehum", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(200, "/text/plain", String(humiditeValue)); });

  server.on("/courbelum", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(200, "/text/plain", String(luminositeValue)); });
  server.on("/set_led_intensity", HTTP_GET, [](AsyncWebServerRequest *request)
            {
              if (request->hasParam("intensity")) {
                String res = request->getParam("intensity")->value();
                int intensity = res.toInt();
                Serial.println(res);
                analogWrite(21,intensity);
              }
              request->send(200, "text/plain", "OK"); });
  server.on("/minute", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(200, "/text/plain", String(minuteValue)); });
  server.on("/heures", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(200, "/text/plain", String(heureValue)); });
  server.on("/set_datetime", HTTP_GET, [](AsyncWebServerRequest *request)
            {
              // Vérifie si tous les paramètres sont présents
              if (request->hasParam("day") && request->hasParam("time") && request->hasParam("date")) {
                String day = request->getParam("day")->value();
                String time = request->getParam("time")->value();
                String date = request->getParam("date")->value();
                uint8_t hh = time.substring(0, 2).toInt();
                uint8_t mm = time.substring(3, 5).toInt();
                uint8_t index = day.toInt();
                uint16_t year = date.substring(0, 4).toInt();    
                uint8_t month = date.substring(5, 7).toInt();    
                uint8_t day_ = date.substring(8, 10).toInt();     
                myRTC3231.setTime(hh, mm, ss);
                myRTC3231.setDate(index,day_,month,year);
                request->send(200, "text/plain", "Date et heure reçues avec succès");
              } else {
                request->send(400, "text/plain", "Paramètres manquants");
              } });

  server.on("/getstatetemp", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(200, "/text/plain", String(_StateDS1621)); });
  server.on("/getstatehumidity", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(200, "/text/plain", String(_StateAHT20)); });
  server.on("/getstatelight", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(200, "/text/plain", String(_StateBH1750)); });
  server.on("/getstatedatetime", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(200, "/text/plain", String(_StateRTC3231)); });
  server.on("/getstateSDcarte", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(200, "/text/plain", String(_StateSAVE)); });
  server.on("/getstateWifi", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(200, "/text/plain", String(_StateWIFI)); });
  server.on("/set_city", HTTP_GET, [this](AsyncWebServerRequest *request)
            {
                zeCity = request->getParam("city")->value();  
                Serial.println(zeCity);
                String TheURL = URL + zeCity + "&appid=" +  API_KEY + UNITS + LANG;
                Serial.println(TheURL);
                String payload = httpGetRequest(TheURL);
                request->send(200,"application/json", payload); });
}
String CONFWIFI::httpGetRequest(String url)
{
  {
    WiFiClient client;
    HTTPClient http;
    String payload = "";
    //
    http.begin(client, url);
    int httpResponseCode = http.GET();
    if (httpResponseCode > 0)
    {
      Serial.print("HTTP code:");
      Serial.println(httpResponseCode);
      payload = http.getString();
      Serial.println(payload);
    }
    //
    http.end();
    return payload;
  }
}