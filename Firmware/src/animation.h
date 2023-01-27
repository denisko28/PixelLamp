#include <Arduino.h>
#include <FastLED.h>

void setAnimationInterval(int timeInterval);

void loadImage(const uint32_t (*frame), CRGB *leds);

void animation(CRGB *leds, const uint32_t *const * animBitmapArray, size_t size);

void cylonAnimation(CRGB *leds);