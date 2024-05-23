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

void displayStats(int averageBPM, unsigned long monitoringDuration) {
  lcd.clear();
  displayAverageBPM(averageBPM);
  displayDuration(monitoringDuration);
  lcd.setCursor(6, 3);
  lcd.print("Press to RESET");
}

void displayAverageBPM(int averageBPM) {
  lcd.setCursor(0, 0);
  lcd.print("Average BPM - ");
  lcd.print(averageBPM);
}

void displayDuration(unsigned long monitoringDuration) {

  int hours = monitoringDuration / 3600;
  int minutes = monitoringDuration / 60;
  int seconds = monitoringDuration % 60;

  lcd.setCursor(0, 1);
  lcd.print("Duration - ");
  lcd.print(hours < 10 ? "0" : "");
  lcd.print(hours);
  lcd.print(":");
  lcd.print(minutes < 10 ? "0" : "");
  lcd.print(minutes);
  lcd.print(":");
  lcd.print(seconds < 10 ? "0" : "");
  lcd.print(seconds);
}
