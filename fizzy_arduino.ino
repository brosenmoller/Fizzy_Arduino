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
const float MIN_LED_DURATION = 0.3;
const float HIT_COOLDOWN = 0.5;

int currentIteration = 0;
AccelerometerData currentDataGroup[ITERATION_GROUP_SIZE];
AccelerometerData currentMeanData;
bool hasMean = false;

unsigned long lastMillis = 0;
float ledTimer = -1;

uint16_t hue = 0;
float hitCooldownTimer = -1;

void setup() {
  Serial.begin(9600);
  while (!Serial);
  Serial.println("Started");

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

  if (ledTimer < 0) {
    // Turn of led if needed
  } else {
    ledTimer -= deltaTime;
  }

  if (hitCooldownTimer > 0) {
    hitCooldownTimer -= deltaTime;
    return;
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

    // Detect hit based on acceleration change
    if (abs(newMeanData.x - currentMeanData.x) > MAX_DIFFERENCE ||
        abs(newMeanData.y - currentMeanData.y) > MAX_DIFFERENCE ||
        abs(newMeanData.z - currentMeanData.z) > MAX_DIFFERENCE)
    {
      ledTimer = MIN_LED_DURATION;
      hitCooldownTimer = HIT_COOLDOWN;
      hue += 32; // Shift hue for a visible color change
      hue %= 256;

      for (int i = 0; i < NUMPIXELS; i++) {
        strip.setPixelColor(i, strip.ColorHSV(hue * 256)); // Adafruit uses 0-65535 for HSV
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