#ifndef DISPLAYMETHODS_H
#define DISPLAYMETHODS_H


void setupDisplay();

void displayMissingText(bool fingerDetected);

void displayInitialText();
void displayErrorText();
void displayNoFingerDetectedText();
void displayWarmingUpText();
void displayMonitoringText(int BPM);

void displayStats(int averageBPM, unsigned long monitoringDuration);
void displayAverageBPM(int averageBPM);
void displayDuration(unsigned long monitoringDuration);

#endif