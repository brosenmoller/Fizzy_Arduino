#ifndef ACCELEROMETER_H
#define ACCELEROMETER_H

#include "Arduino_BMI270_BMM150.h"

const int ITERATION_GROUP_SIZE = 5;
const float ACCELEROMETER_HIT_THRESHOLD = 0.9;
const float HIT_COOLDOWN = 0.5;

struct AccelerometerData {
  float x;
  float y;
  float z;
};

AccelerometerData currentDataGroup[ITERATION_GROUP_SIZE];
AccelerometerData currentMeanData;
int currentIteration = 0;

bool hasMean = false;
float hitCooldownTimer = -1;

void setup_accelerometer() {
  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
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

bool has_ball_been_hit(float deltaTime) {
  if (hitCooldownTimer >= 0) {
    hitCooldownTimer -= deltaTime;
  }

  if (!IMU.accelerationAvailable()) { return false; }

  float x, y, z;
  IMU.readAcceleration(x, y, z);
  AccelerometerData data = {x, y, z};
  currentDataGroup[currentIteration] = data;

  currentIteration++;
  if (currentIteration < ITERATION_GROUP_SIZE) { return false; }

  currentIteration = 0;
  AccelerometerData newMeanData = GetMeanOfCurrentDataGroup();

  if (!hasMean) {
    currentMeanData = newMeanData;
    hasMean = true;
    return false;
  }

  float dx = abs(newMeanData.x - currentMeanData.x);
  float dy = abs(newMeanData.y - currentMeanData.y);
  float dz = abs(newMeanData.z - currentMeanData.z);
  bool hasDetectedHit = dx > ACCELEROMETER_HIT_THRESHOLD || dy > ACCELEROMETER_HIT_THRESHOLD || dz > ACCELEROMETER_HIT_THRESHOLD;
  bool isAllowedToHit = hitCooldownTimer < 0;

  if (hasDetectedHit && isAllowedToHit)
  {
    hitCooldownTimer = HIT_COOLDOWN;
    return true;
  }

  currentMeanData = newMeanData;
  return false;
}

#endif