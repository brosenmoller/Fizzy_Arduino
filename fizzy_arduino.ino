#include "utility.h"
#include "accelerometer.h"
#include "led_strip.h"

const int SPEAKER_PIN = 7;

void setup() {
  Serial.begin(9600);
  setup_led_strip();
  setup_accelerometer();
}

void loop() {
  UpdateDeltaTime()
  UpdateButtonState()

  bool wasBallHit = has_ball_been_hit(deltaTime);
  
  
}