#include "main.h"
#include "animation.h"
#include "utils.h"
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include "ArduinoJson.h"
#include "customEnums.h"
#include "bulbBitmap.h"
#include "heartBitmap.h"
#include "faceBitmap.h"
#include "staticBitmaps.h"

// Wi-Fi
const char *ssid = "FoxNet 2.4";
const char *password = "98345458";

IPAddress ip(192, 168, 0, 109);
IPAddress gateway(192, 168, 0, 1);
IPAddress subnet(255, 255, 255, 0);
ESP8266WebServer server(82);

// Define the array of leds
CRGB leds[NUM_LEDS];

//Current mode
uint8_t currentMode = 0;
bool currentModeChanged = false;

void setup()
{
  Serial.begin(57600);
  Serial.println("resetting");
  FastLED.addLeds<WS2812B, LED_DT, GRB>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);
  if (CURRENT_LIMIT > 0)
    FastLED.setMaxPowerInVoltsAndMilliamps(5, CURRENT_LIMIT);
  FastLED.clear(true);
  FastLED.show();

  // Выполнить подключение к сети
  connectToNetwork();
}

void loop()
{
  // animation(leds);
  // FastLED.show();
  switch (currentMode)
  {
    case HEART_ANIMATION_MODE:
      animation(leds, heart_array, sizeof(heart_array) / sizeof(heart_array[0]));
      break;
    case FACE_ANIMATION_MODE: 
      animation(leds, face_array, sizeof(face_array) / sizeof(face_array[0]));
      break;
    case BULB_ANIMATION_MODE: 
      animation(leds, bulb_array, sizeof(bulb_array) / sizeof(bulb_array[0]));
      break;
    case LIGHT_MODE: 
        fill_solid(leds, NUM_LEDS, 0xFFFFFF);
        FastLED.show();
      break;
    case BULB_MODE:
        for(byte i = 0; i < NUM_LEDS; i++){
          drawPixel(i, leds, bulb_array[0][i]);
        }
        FastLED.show();
      break;
    case STRAWBERRY_MODE:
        for(byte i = 0; i < NUM_LEDS; i++){
          drawPixel(i, leds, strawberry[i]);
        }
        FastLED.show();
      break;
    case PINEAPPLE_MODE:
        for(byte i = 0; i < NUM_LEDS; i++){
          drawPixel(i, leds, pineapple[i]);
        }
        FastLED.show();
      break;
    case CYLON_ANIMATION_MODE:
      cylonAnimation(leds);
      break;
  }
  server.handleClient();
}

void setCurrentMode(uint8_t newMode) {
  currentMode = newMode;
  currentModeChanged = true;
}

bool isBodyRecieved() {
  if (server.hasArg("plain") == false)
  {
    server.send(200, "text/plain", "Body not received");
    return false;
  }
  return true;
}

void drawPictureHandler()
{
  if(currentMode != DRAWING_MODE) {
    server.send(200, "text/plain", "picture not set, change mode to drawing");
    return;
  }

  if(!isBodyRecieved())
    return;

  StaticJsonDocument<2000> doc;
  deserializeJson(doc, server.arg("plain"));

  uint32_t arr[68];
  Serial.println(server.arg("plain"));
  copyArray(doc["array"], arr);

  server.send(200, "text/plain", "picture set successfuly");
  FastLED.clear(true);
  for(byte i = 0; i < NUM_LEDS; i++){
    drawPixel(i, leds, arr[i]);
  }
  FastLED.show();
}

void setModeHandler() {
  if(!isBodyRecieved())
    return;
  
  StaticJsonDocument<30> doc;
  deserializeJson(doc, server.arg("plain"));
  setCurrentMode(doc["modeIndex"]);
  server.send(200, "text/plain", "mode successfuly set");
}

void setAnimIntervalHandler() {
  if(!isBodyRecieved())
    return;
  
  StaticJsonDocument<30> doc;
  deserializeJson(doc, server.arg("plain"));
  setAnimationInterval(doc["animInterval"]);
  server.send(200, "text/plain", "animation interval successfuly set");
}

void configureEndpoints()
{
  server.on("/drawPicture", drawPictureHandler);
  server.on("/setMode", setModeHandler);
  server.on("/setAnimInterval", setAnimIntervalHandler);

  server.on("/", []{
    server.send(200, "text/plain", "message");
  });
}

void connectToNetwork()
{
  WiFi.begin(ssid, password);
  WiFi.config(ip, gateway, subnet);

  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  configureEndpoints();
  server.enableCORS(true);
  server.begin();
}