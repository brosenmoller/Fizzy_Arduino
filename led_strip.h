#ifndef LED_STRIP_H
#define LED_STRIP_H

#include <Adafruit_NeoPixel.h>

struct Color {
  float r;
  float g;
  float b;
};

const int LED_STRIP_PIN = 5;
const int LED_STRIP_LED_COUNT = 30;

Adafruit_NeoPixel strip(LED_STRIP_LED_COUNT, LED_STRIP_PIN, NEO_GRB + NEO_KHZ800);

void setup_led_strip() {
  strip.begin();
  strip.setBrightness(150);
  strip.show();
}

void set_strip_color(float r, float g, float b, float number = LED_STRIP_LED_COUNT) {
  for (int i = 0; i < number; i++) {
    strip.setPixelColor(i, r, g, b);
  }
  strip.show();
}

#endif