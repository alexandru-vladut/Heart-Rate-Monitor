#ifndef DISPLAYMETHODS_H
#define DISPLAYMETHODS_H


void setupDisplay();

void displayMissingText(bool fingerDetected);

void displayInitialText();
void displayErrorText();
void displayNoFingerDetectedText();
void displayWarmingUpText();
void displayMonitoringText(int BPM);

void displayStats(int averageBPM, int maxBPM, unsigned long monitoringDuration);
void displayBPM(int averageBPM, int maxBPM);
void displayDuration(unsigned long monitoringDuration);
void displayMessage(int averageBPM);

#endif