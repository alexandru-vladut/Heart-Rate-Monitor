#include <Wire.h>

#include "MAX30105.h"
#include "heartRate.h"

#include "displayMethods.h"
 

// RGB LED setup
#define RED_PIN 5
#define GREEN_PIN 6
#define BLUE_PIN 7
                  
MAX30105 pulseSensor;  // Creates an instance of the MAX30105 object called "pulseSensor"

volatile int buttonState = 1;  // State of the button, initially HIGH (not pressed)
volatile bool buttonPressed = false;

bool isMonitoring = false;
bool isWarmingUp = false;

bool lastFingerDetected = false;
bool fingerDetected = false;

const byte ARRAY_SIZE = 6; // Increase this for more averaging
byte rawBPMs[ARRAY_SIZE]; // Array of heart rates
byte index = 0;
long lastBeat = 0; // Time at which the last beat occurred

int displayedBPM;

int beatCount = 0;  // Count of beats detected
int totalBPM = 0;   // Total of BPMs collected

int beatCountAux = 0;

unsigned long startTime = 0;           // Start time of monitoring
unsigned long monitoringDuration = 0;  // Duration of monitoring


void setup() {

  // Serial Monitor
  Serial.begin(115200);

  // LCD setup
  setupDisplay();

  // Button setup
  pinMode(2, INPUT_PULLUP);
  // Attach interrupt to pin 2 (digital pin 2 on most Arduino boards is interrupt 0)
  attachInterrupt(digitalPinToInterrupt(2), buttonISR, CHANGE);

  // RGB LED setup
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
  analogWrite(RED_PIN, 255);  // Turn off the LED (common anode, so HIGH is off)
  analogWrite(GREEN_PIN, 255);  // Turn off the LED (common anode, so HIGH is off)
  analogWrite(BLUE_PIN, 255);  // Turn off the LED (common anode, so HIGH is off)

  // Pulse sensor setup
  if (!pulseSensor.begin(Wire, I2C_SPEED_FAST)) { //Use default I2C port, 400kHz speed
    Serial.println("MAX30105 was not found. Please check wiring/power. ");
    while (1);
  }
  pulseSensor.setup(); //Configure sensor with default settings
  pulseSensor.setPulseAmplitudeRed(0x0A); //Turn Red LED to low to indicate sensor is running
  pulseSensor.setPulseAmplitudeGreen(0);  // Turn off Green LED

  // Start in idle state
  displayInitialText();
}

void loop() {

  if (buttonPressed) {

    buttonPressed = false;
    isMonitoring = !isMonitoring;

    if (isMonitoring) {

      Serial.println("START");
      Serial.println("---------------------------------------------");

      beatCount = 0;  // Reset beat count
      totalBPM = 0;   // Reset total BPM
      startTime = millis();  // Record start time

      beatCountAux = 0;

      isWarmingUp = true;

      // Bug fix: Some messages don't get displayed at the beginning of current monitoring session,
      // based of the exit value of fingerDetected in previous monitoring session.
      displayMissingText(fingerDetected);

    } else {

      Serial.println("STOP");
      Serial.println();

      analogWrite(RED_PIN, 255); // Turn off LED
      computeStats();

    }
  }

  if (isMonitoring) {
    monitorHeartbeat();
  }
}

void buttonISR() {
  static unsigned long lastInterruptTime = 0;
  unsigned long interruptTime = millis();
  
  if (interruptTime - lastInterruptTime > 50) {  // Debouncing logic
    int currentState = digitalRead(2);

    if (currentState == HIGH && buttonState == LOW) {
      // Full button press detected (down and up)
      buttonPressed = true;
    }

    buttonState = currentState;
  }
  
  lastInterruptTime = interruptTime;
}

void monitorHeartbeat() {

  long irValue = pulseSensor.getIR();

  if (irValue < 50000) { // finger not detected

    if (fingerDetected) { // detects Finger detected -> Finger NOT detected transition
      displayNoFingerDetectedText();
    }

    fingerDetected = false;

  } else { // finger detected

    if (!fingerDetected) { // detects Finger NOT detected -> Finger detected transition
      if (isWarmingUp) {
        displayWarmingUpText();
      } else {
        displayMonitoringText(displayedBPM); // display last BPM found until the sensor finds a new heartbeat
      }
    }

    if (checkForBeat(irValue)) { // heart beat detected

      float rawBPM = getBPM();

      Serial.print("Raw BPM: ");
      Serial.print(rawBPM);

      if (rawBPM > 50 && rawBPM < 100) { // if rawBPM is accurate

        Serial.println(" (CONSIDERED)");

        displayedBPM = getDisplayedBPM(rawBPM);

        if (!isWarmingUp) {
          // Compute final stats
          totalBPM += displayedBPM;
          beatCount++;
        }

        beatCountAux++;

        // Turn off warming up mode after $ARRAY_SIZE accurate beats captured
        if (beatCountAux > ARRAY_SIZE) {
          isWarmingUp = false;
        }

      } else {
        Serial.println(" (IGNORED)");
      }


      if (!isWarmingUp) {
        Serial.print("Heartbeat detected, BPM: ");
        Serial.println(displayedBPM);

        displayMonitoringText(displayedBPM);
        blinkLED();

      } else {
        Serial.println("Heartbeat detected, warming up...");
      }

      Serial.println("---------------------------------------------");
      
    }

    fingerDetected = true;
  }
}

// Get BPM based on time between heartbeats (delta)
// Represents the raw BPM value captured by the sensor,
// needs refinement because of noise and similar factors.
float getBPM() {
  long delta = millis() - lastBeat;
  lastBeat = millis();
  return 60 / (delta / 1000.0);
}

// Get BPM that will be displayed on LCD.
// Represents a refined BPM value by averaging the last $ARRAY_SIZE raw BPM values.
int getDisplayedBPM(float rawBPM) {

  rawBPMs[index++] = (byte)rawBPM; // Store this reading in the array
  index = index % ARRAY_SIZE;

  // Take average of readings
  int avgBPM = 0;
  for (byte i = 0 ; i < ARRAY_SIZE ; i++)
    avgBPM += rawBPMs[i];
  avgBPM = avgBPM / ARRAY_SIZE;

  return avgBPM;
}

void blinkLED() {
  for (int fadeValue = 0; fadeValue <= 255; fadeValue += 5) {  // Increase fadeValue from brightness to off
    analogWrite(RED_PIN, fadeValue);  // Apply fade effect
    delay(10);
  }
}

void computeStats() {

  if (beatCount > 0) {

    int averageBPM = totalBPM / beatCount;
    monitoringDuration = (millis() - startTime) / 1000;

    displayStats(averageBPM, monitoringDuration);

  } else {
    displayErrorText();
  }
  
}
