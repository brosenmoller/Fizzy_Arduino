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

void DetectClicks() {
  static unsigned long lastCheckTime = 0;

  if (buttonIsPressedThisLoop) {
    clickCount++;
    lastButtonPressTime = currentMillis;
  }

  if (clickCount > 0 && currentMillis - lastButtonPressTime > clickInterval) {
    if (clickCount == 1) {
      Serial.println("Single Click");
    } else if (clickCount == 2) {
      Serial.println("Double Click");
    } else if (clickCount == 3) {
      Serial.println("Triple Click");
    } else {
      Serial.print(clickCount);
      Serial.println(" Clicks detected");
    }

    clickCount = 0;
  }
}

#endif


const int BUTTON_PIN = 6;

bool buttonWasPressed = false;
bool buttonIsPressed = false;
bool buttonIsPressedThisLoop = false;

unsigned long lastButtonPressTime = 0;   // Time of last press
const unsigned long clickInterval = 400; // Max time between clicks (ms)

int clickCount = 0;        // Tracks clicks in progress
int confirmedClickCount = 0; // Stores the final click count for reading
bool clickReady = false;   // True when confirmedClickCount is ready

void setup() {
  pinMode(BUTTON_PIN, INPUT);
  Serial.begin(9600);
}

void loop() {
  UpdateButtonState();
  DetectClicks();

  if (clickReady) {
    if (confirmedClickCount == 1) {
      Serial.println("Single Click");
    } else if (confirmedClickCount == 2) {
      Serial.println("Double Click");
    } else if (confirmedClickCount == 3) {
      Serial.println("Triple Click");
    } else if (confirmedClickCount > 3) {
      Serial.print(confirmedClickCount);
      Serial.println(" Clicks");
    }

    clickReady = false;
  }
}

void UpdateButtonState() {
  buttonIsPressed = digitalRead(BUTTON_PIN) == HIGH;
  buttonIsPressedThisLoop = false;

  if (buttonIsPressed && !buttonWasPressed) {
    buttonIsPressedThisLoop = true;
  }

  buttonWasPressed = buttonIsPressed;
}

void DetectClicks() {
  if (buttonIsPressedThisLoop) {
    clickCount++;
    lastButtonPressTime = millis();
  }

  if (clickCount > 0 && millis() - lastButtonPressTime > clickInterval) {
    confirmedClickCount = clickCount; // Save final count
    clickReady = true;                // Mark it ready to read
    clickCount = 0;                   // Reset for next detection
  }
}