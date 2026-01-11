#ifndef UTILITY_H
#define UTILITY_H

const int BUTTON_PIN = 6;

unsigned long lastMillis = 0;
float deltaTime = 0;

bool buttonWasPressed = false;
bool buttonIsPressed = false;
bool buttonIsPressedThisLoop = false;

void UpdateDeltaTime() {
  unsigned long currentMillis = millis();
  float deltaTime = (currentMillis - lastMillis) / 1000.0;
  lastMillis = currentMillis;
}

void UpdateButtonState(){
  bool buttonIsPressed = digitalRead(BUTTON_PIN) == HIGH;
  bool buttonIsPressedThisLoop = false;

  if (buttonIsPressed && !buttonWasPressed) {
    buttonIsPressedThisLoop = true;
  }

  buttonWasPressed = buttonIsPressed;
}

#endif