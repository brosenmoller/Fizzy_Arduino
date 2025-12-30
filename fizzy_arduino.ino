#include <Adafruit_NeoPixel.h>
#include "Arduino_BMI270_BMM150.h"

#define STRIP_PIN 5
#define NUMPIXELS 30

Adafruit_NeoPixel strip(NUMPIXELS, STRIP_PIN, NEO_GRB + NEO_KHZ800);

struct AccelerometerData {
  float x;
  float y;
  float z;
};

const int ITERATION_GROUP_SIZE = 5;
const float MAX_DIFFERENCE = 0.9;
const float HIT_COOLDOWN = 0.5;

int currentIteration = 0;
AccelerometerData currentDataGroup[ITERATION_GROUP_SIZE];
AccelerometerData currentMeanData;
bool hasMean = false;

unsigned long lastMillis = 0;

uint16_t hue = 0;
float hitCooldownTimer = -1;

void setup() {
  Serial.begin(9600);

  strip.begin();
  strip.setBrightness(150);
  strip.show();

  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }
}

void loop() {
  float deltaTime = (millis() - lastMillis) / 1000.0;
  lastMillis = millis();

  if (hitCooldownTimer >= 0) {
    hitCooldownTimer -= deltaTime;
  }

  if (!IMU.accelerationAvailable()) { return; }

  float x, y, z;
  IMU.readAcceleration(x, y, z);
  AccelerometerData data = {x, y, z};
  currentDataGroup[currentIteration] = data;

  currentIteration++;
  if (currentIteration >= ITERATION_GROUP_SIZE) {
    currentIteration = 0;

    AccelerometerData newMeanData = GetMeanOfCurrentDataGroup();

    if (!hasMean) {
      currentMeanData = newMeanData;
      hasMean = true;
      return;
    }

    float dx = abs(newMeanData.x - currentMeanData.x);
    float dy = abs(newMeanData.y - currentMeanData.y);
    float dz = abs(newMeanData.z - currentMeanData.z);
    bool hasDetectedHit = dx > MAX_DIFFERENCE || dy > MAX_DIFFERENCE || dz > MAX_DIFFERENCE;
    bool isAllowedToHit = hitCooldownTimer < 0;

    if (hasDetectedHit && isAllowedToHit)
    {
      hitCooldownTimer = HIT_COOLDOWN;
      hue += 32; // Shift hue for a visible color change
      hue %= 256;

      uint16_t scaledHue = map(hue, 0, 255, 0, 65535);
      for (int i = 0; i < NUMPIXELS; i++) {
        strip.setPixelColor(i, strip.ColorHSV(scaledHue));
      }
      strip.show();
    }

    currentMeanData = newMeanData;
  }
}

AccelerometerData GetMeanOfCurrentDataGroup() {
  AccelerometerData meanData = {0, 0, 0};
  for (int i = 0; i < ITERATION_GROUP_SIZE; i++) {
    AccelerometerData data = currentDataGroup[i];
    meanData.x += data.x;
    meanData.y += data.y;
    meanData.z += data.z;
  }
  meanData.x /= ITERATION_GROUP_SIZE;
  meanData.y /= ITERATION_GROUP_SIZE;
  meanData.z /= ITERATION_GROUP_SIZE;
  return meanData;
}