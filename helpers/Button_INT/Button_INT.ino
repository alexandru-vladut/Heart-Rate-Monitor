#include <Wire.h>
#include <LiquidCrystal_I2C.h> 

LiquidCrystal_I2C lcd(0x27, 20, 4);  // set the LCD address to 0x27 for a 20 chars and 4 line display                      

volatile int buttonState = 1;  // State of the button, initially HIGH (not pressed)
volatile bool buttonPressed = false;
bool isMonitoring = false;

// RGB LED setup
#define RED_PIN 5
#define GREEN_PIN 6
#define BLUE_PIN 7

// Brightness level (0-255)
const int brightness1 = 50;
const int brightness2 = 255;


void setup() {

  // Serial Monitor
  Serial.begin(115200);

  // LCD setup
  lcd.init();
  lcd.backlight();

  // Button setup
  pinMode(2, INPUT_PULLUP);

  // RGB LED setup
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
  analogWrite(RED_PIN, 255);  // Turn off the LED (common anode, so HIGH is off)
  analogWrite(GREEN_PIN, 255);  // Turn off the LED (common anode, so HIGH is off)
  analogWrite(BLUE_PIN, 255);  // Turn off the LED (common anode, so HIGH is off)

  // Attach interrupt to pin 2 (digital pin 2 on most Arduino boards is interrupt 0)
  attachInterrupt(digitalPinToInterrupt(2), buttonISR, CHANGE);

  // Start in idle state
  displayInitialText();
}

void loop() {

  if (buttonPressed) {
    buttonPressed = false;

    isMonitoring = !isMonitoring;

    if (isMonitoring) {
      displayMonitoringText();
    } else {
      displayInitialText();
    }

    blinkAndFadeOut();
  }

  // if (isMonitoring) {
  //   // Turn on the red LED with specified brightness
  //   analogWrite(RED_PIN, 255 - brightness2);  // Common anode, so invert the brightness
  //   analogWrite(GREEN_PIN, 255);  // Turn off green
  //   analogWrite(BLUE_PIN, 255);  // Turn off blue
  // } else {
  //   // Turn off the RGB LED
  //   analogWrite(RED_PIN, 255 - brightness1);
  //   analogWrite(GREEN_PIN, 255);
  //   analogWrite(BLUE_PIN, 255);
  // }
}

void blinkAndFadeOut() {
  // Fade out the LED
  for (int fadeValue = 255 - brightness2; fadeValue <= 255; fadeValue += 5) {  // Increase fadeValue from brightness to off
    analogWrite(RED_PIN, fadeValue);  // Apply fade effect
    delay(10);
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

void displayInitialText() {
  lcd.clear();
  lcd.setCursor(0, 1);
  lcd.print("Press button to");
  lcd.setCursor(0, 2);
  lcd.print("start monitoring.");
}

void displayMonitoringText() {
  lcd.clear();
  lcd.setCursor(0, 1);
  lcd.print("Monitoring...");
}
