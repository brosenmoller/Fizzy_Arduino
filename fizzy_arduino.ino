#include "accelerometer.h"
#include "led_strip.h"

const int BUTTON_PIN = 6;
const int SPEAKER_PIN = 7;

unsigned long lastMillis = 0;

void setup() {
  Serial.begin(9600);
  setup_led_strip();
  setup_accelerometer();
}

void loop() {
  unsigned long currentMillis = millis();
  float deltaTime = (currentMillis - lastMillis) / 1000.0;
  lastMillis = currentMillis;
  
  bool wasBallHit = has_ball_been_hit(deltaTime);
}