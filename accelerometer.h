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

void SetupAccelerometer() {
  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }
}

AccelerometerData GetMeanOfDataGroup(const AccelerometerData* dataGroup) 
{
  AccelerometerData meanData = {0, 0, 0};

  for (int i = 0; i < ITERATION_GROUP_SIZE; i++) {
    meanData.x += dataGroup[i].x;
    meanData.y += dataGroup[i].y;
    meanData.z += dataGroup[i].z;
  }

  meanData.x /= ITERATION_GROUP_SIZE;
  meanData.y /= ITERATION_GROUP_SIZE;
  meanData.z /= ITERATION_GROUP_SIZE;
  return meanData;
}

bool HasBallBeenHit(float deltaTime) {
  static AccelerometerData dataGroup[ITERATION_GROUP_SIZE];
  static AccelerometerData meanData;
  static int iteration = 0;
  static bool hasMean = false;
  static float cooldown = -1.0f;

  if (cooldown >= 0) {
    cooldown -= deltaTime;
  }

  if (!IMU.accelerationAvailable()) { return false; }

  float x, y, z;
  IMU.readAcceleration(x, y, z);
  dataGroup[currentIteration] = {x, y, z};
  iteration++;

  if (currentIteration < ITERATION_GROUP_SIZE) { return false; }

  iteration = 0;
  AccelerometerData newMeanData = GetMeanOfDataGroup(dataGroup);

  if (!hasMean) {
    meanData = newMeanData;
    hasMean = true;
    return false;
  }

  float dx = abs(newMeanData.x - currentMeanData.x);
  float dy = abs(newMeanData.y - currentMeanData.y);
  float dz = abs(newMeanData.z - currentMeanData.z);
  bool hasDetectedHit = dx > ACCELEROMETER_HIT_THRESHOLD || dy > ACCELEROMETER_HIT_THRESHOLD || dz > ACCELEROMETER_HIT_THRESHOLD;
  bool isAllowedToHit = cooldown < 0;

  if (hasDetectedHit && isAllowedToHit)
  {
    cooldown = HIT_COOLDOWN;
    return true;
  }

  meanData = newMeanData;
  return false;
}

#endif