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

void SetupLedStrip() {
  strip.begin();
  strip.setBrightness(150);
  ClearStrip();
  strip.show();
}

void SetStripColor(float r, float g, float b, float number = LED_STRIP_LED_COUNT) {
  for (int i = 0; i < number; i++) {
    strip.setPixelColor(i, r, g, b);
  }
  strip.show();
}

void ClearStrip() {
  strip.clear();
  strip.show();
}

Color LerpColor(Color a, Color b, float t) {
  return {
    a.r + (b.r - a.r) * t,
    a.g + (b.g - a.g) * t,
    a.b + (b.b - a.b) * t
  };
}


void RainbowFade(unsigned long now) {
  static const Color rainbowFadePalette[] = {
    {255, 0, 0},     // Red
    {255, 128, 0},   // Orange
    {255, 255, 0},   // Yellow
    {0, 255, 0},     // Green
    {0, 255, 255},   // Cyan
    {0, 0, 255},     // Blue
    {128, 0, 255}    // Purple
  };

  static const int paletteSize = sizeof(rainbowFadePalette) / sizeof(Color);

  static int currentIndex = 0;
  static unsigned long lastColorChange = 0;
  static const unsigned long fadeDuration = 2000;

  float t = (now - lastColorChange) / (float)fadeDuration;

  if (t >= 1.0f) {
    t = 0.0f;
    lastColorChange = now;
    currentIndex = (currentIndex + 1) % paletteSize;
  }

  Color from = rainbowFadePalette[currentIndex];
  Color to   = rainbowFadePalette[(currentIndex + 1) % paletteSize];

  SetStripColor(LerpColor(from, to, t));
}

#endif