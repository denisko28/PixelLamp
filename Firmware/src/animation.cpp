#include "animation.h"
#include "utils.h"
#include "../lib/timerMinim.h"
#include "main.h"

#define D_GIF_SPEED 100

timerMinim gifTimer(D_GIF_SPEED);
byte frameNum;
static uint8_t hue = 0;
bool reverseAnim = false;

void setAnimationInterval(int timeInterval) {
  gifTimer.setInterval(timeInterval);
}

void loadImage(const uint32_t (*frame), CRGB *leds) {
  for (byte i = 0; i < NUM_LEDS; i++) {
    drawPixel(i, leds, frame[i]);
  }
}

void animation(CRGB *leds, const uint32_t *const * animBitmapArray, size_t size) {
  boolean isReady = gifTimer.isReady();
  if (isReady) {
    if (frameNum >= size) frameNum = 0;
    loadImage(animBitmapArray[frameNum], leds);
    frameNum++;
    FastLED.show();
  }
}

void fadeall(CRGB *leds) { for(int i = 0; i < NUM_LEDS; i++) { leds[i].nscale8(250); } }

void cylonAnimation(CRGB *leds) {
  if (gifTimer.isReady()) {
    if(reverseAnim)
      leds[NUM_LEDS - frameNum] = CHSV(hue++, 255, 255);
    else
      leds[frameNum] = CHSV(hue++, 255, 255);
    if (++frameNum >= NUM_LEDS){ 
      frameNum = 0;
      reverseAnim = !reverseAnim;
    }
    FastLED.show();
    fadeall(leds);
  }
}