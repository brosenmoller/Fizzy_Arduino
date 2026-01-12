#ifndef UTILITY_H
#define UTILITY_H

unsigned long currentMillis = 0;
unsigned long lastMillis = 0;
float deltaTime = 0;

void UpdateDeltaTime() {
  currentMillis = millis();
  deltaTime = (currentMillis - lastMillis) / 1000.0;
  lastMillis = currentMillis;
}

const int BUTTON_PIN = 6;

bool buttonWasPressed = false;
bool buttonIsPressed = false;
bool buttonIsPressedThisLoop = false;

unsigned long lastButtonPressTime = 0; 
const unsigned long clickInterval = 400;

int clickCount = 0;

void UpdateButtonState(){
  buttonIsPressed = digitalRead(BUTTON_PIN) == HIGH;
  buttonIsPressedThisLoop = false;

  if (buttonIsPressed && !buttonWasPressed) {
    buttonIsPressedThisLoop = true;
  }

  buttonWasPressed = buttonIsPressed;
}

#endif