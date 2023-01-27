#include "utils.h"
#include "main.h"

uint32_t expandColor(uint16_t color) {
  return ((uint32_t)pgm_read_byte(&gamma5[ color >> 11       ]) << 16) |
         ((uint32_t)pgm_read_byte(&gamma6[(color >> 5) & 0x3F]) <<  8) |
         pgm_read_byte(&gamma5[ color       & 0x1F]);
}

// гамма-коррекция (более натуральные цвета)
uint32_t gammaCorrection(uint32_t color) {
  byte r = (color >> 16) & 0xFF;  // Extract the RR byte
  byte g = (color >> 8) & 0xFF;   // Extract the GG byte
  byte b = color & 0xFF;        // Extract the BB byte

  r = pgm_read_byte(&gammaR[r]);
  g = pgm_read_byte(&gammaG[g]);
  b = pgm_read_byte(&gammaB[b]);

  uint32_t newColor = ((long)(r & 0xff) << 16) + ((long)(g & 0xff) << 8) + ((long)b & 0xff);
  return newColor;
}

void drawPixel(uint8_t pixelIndex, CRGB *leds, uint32_t color) {
  byte i = 0;
  bool finish = false;
  for (byte y = 0; y < STRIPS_NUM; y++) {
    byte t = i;
    byte n = STRIP_LENGTHES[y];
    for(byte x = 0; x < n; x++){
      if(i == pixelIndex) {
        if(y%2 != 0) {
          leds[t + n - x -1] = gammaCorrection(color);
        } else
          leds[i] = gammaCorrection(color);
        finish = true;
        break;
      }
      i++;
    }
    if (finish)
      break;
  }
}

void drawPixelXY(uint8_t xParam, uint8_t yParam, CRGB *leds, uint32_t color) {
  byte i = 0;
  for (byte y = 0; y < STRIPS_NUM; y++) {
    byte t = i;
    Serial.print(STRIP_LENGTHES[y]);
    byte n = STRIP_LENGTHES[y];
    for(byte x = 0; x < n; x++){
      if(x == xParam && y == yParam) {
        if(y%2 != 0)
          leds[t + n - x -1] = gammaCorrection(color);
        else
          leds[i] = gammaCorrection(color);
      }
      i++;
    }
  }
}
