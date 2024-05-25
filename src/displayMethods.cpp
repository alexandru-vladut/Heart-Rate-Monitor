#include "displayMethods.h"

#include <LiquidCrystal_I2C.h>


LiquidCrystal_I2C lcd(0x27, 20, 4);  // set the LCD address to 0x27 for a 20 chars and 4 line display

void setupDisplay() {
  lcd.init();
  lcd.backlight();
}

void displayMissingText(bool fingerDetected) {
  if (fingerDetected) {
    displayWarmingUpText();
  } else {
    displayNoFingerDetectedText();
  }
}

void displayInitialText() {
  lcd.clear();
  lcd.setCursor(5, 1);
  lcd.print("Hi, there!");
  lcd.setCursor(3, 3);
  lcd.print("Press to START"); // 12
}

void displayErrorText() {
  lcd.clear();
  lcd.setCursor(0, 1);
  lcd.print("Not warmed up!");
  lcd.setCursor(6, 3);
  lcd.print("Press to RESET");
}

void displayNoFingerDetectedText() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("No finger detected!");
  lcd.setCursor(0, 2);
  lcd.print("Place your finger");
  lcd.setCursor(0, 3);
  lcd.print("on the sensor.");
}

void displayWarmingUpText() {
  lcd.clear();
  lcd.setCursor(3, 1);
  lcd.print("Warming up...");
}

void displayMonitoringText(int BPM) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Monitoring...");
  lcd.setCursor(0, 1);
  lcd.print("BPM: ");
  lcd.print(BPM);
  lcd.setCursor(7, 3);
  lcd.print("Press to STOP");
}

void displayStats(int averageBPM, int maxBPM, unsigned long monitoringDuration) {
  lcd.clear();
  displayBPM(averageBPM, maxBPM);
  displayDuration(monitoringDuration);
  displayMessage(averageBPM);
}

void displayBPM(int averageBPM, int maxBPM) {
  lcd.setCursor(0, 0);
  lcd.print("BPM: Avg ");
  lcd.print(averageBPM);
  lcd.print(", Max ");
  lcd.print(maxBPM);
}

void displayDuration(unsigned long monitoringDuration) {

  int minutes = monitoringDuration / 60;
  int seconds = monitoringDuration % 60;

  lcd.setCursor(0, 1);
  lcd.print("Duration: ");
  lcd.print(minutes < 10 ? "0" : "");
  lcd.print(minutes);
  lcd.print(":");
  lcd.print(seconds < 10 ? "0" : "");
  lcd.print(seconds);
}

void displayMessage(int averageBPM) {
  lcd.setCursor(0, 3);
  if (averageBPM < 60) {
    lcd.print("Slow and steady!");
  } else if (averageBPM <= 65) {
    lcd.print("Chilly vibes!");
  } else if (averageBPM <= 75) {
    lcd.print("All good!");
  } else if (averageBPM <= 80) {
    lcd.print("Take it easy!");
  } else {
    lcd.print("Slow down!");
  }
}
