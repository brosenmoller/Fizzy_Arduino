#include "utility.h"
#include "accelerometer.h"
#include "led_strip.h"

enum GameState { NO_GAME, GAME };
enum Difficulty { EASY, HARD };
int playerCount = 1;

GameState currentState = NO_GAME;

const int SPEAKER_PIN = 7;

void NoGameState() {
  RainbowFade(currentMillis);
}

void setup() {
  Serial.begin(9600);
  currentState = NO_GAME;
  SetupLedStrip();
  SetupAccelerometer();
}

void loop() {
  UpdateDeltaTime();
  UpdateButtonState();

  bool wasBallHit = HasBallBeenHit(deltaTime);
  
  NoGameState();
}