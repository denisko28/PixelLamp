// #include "bitmap1.h"
// #include "bitmap2.h"
#include <Arduino.h>

#define NUM_LEDS 68 
#define BRIGHTNESS 100
#define CURRENT_LIMIT 1800
#define MATRIX_TYPE 0

// For led chips like Neopixels, which have a data line, ground, and power, you just
// need to define DATA_PIN.  For led chipsets that are SPI based (four wires - data, clock,
// ground, and power), like the LPD8806, define both DATA_PIN and CLOCK_PIN
#define LED_DT D4

#define STRIPS_NUM 10
#define STRIP_LENGTHES (int[]){2, 6, 8, 8, 10, 10, 8, 8, 6, 2}

void connectToNetwork();