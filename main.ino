/*
 * Ultrasonic Simple
 * Prints the distance read by an ultrasonic sensor in
 * centimeters. They are supported to four pins ultrasound
 * sensors (liek HC-SC04) and three pins (like PING)))
 * and Seeed Studio sensors).
 *
 * The circuit:
 * * Module HR-SC04 (four pins) or PING))) (and other with
 *   three pins), attached to digital pins as follows:
 * ---------------------    --------------------
 * | HC-SC04 | Arduino |    | 3 pins | Arduino |
 * ---------------------    --------------------
 * |   Vcc   |   5V    |    |   Vcc  |   5V    |
 * |   Trig  |   5     | OR |   SIG  |   6     |
 * |   Echo  |   6     |    |   Gnd  |   GND   |
 * |   Gnd   |   GND   |    --------------------
 * ---------------------
 * Note: You do not obligatorily need to use the pins defined above
 * 
 * By default, the distance returned by the read()
 * method is in centimeters. To get the distance in inches,
 * pass INC as a parameter.
 * Example: ultrasonic.read(INC)
 *
 *
 * by Blue Francis (github: @am3lue | twitter: @am3lue)
 *
 * This example code is released into the MIT License.
 */

/*
 * Ultrasonic Water Level Monitoring System
 * Displays water volume percentage and status on an I2C LCD.
 *
 * Compatible with 4-pin (HC-SR04) and 3-pin ultrasonic sensors.
 * Author: Blue Francis
 * License: MIT
 */

#include <Ultrasonic.h>
#include <LiquidCrystal_I2C.h>

// =============================
// USER CONFIGURABLE SETTINGS
// =============================
const int TRIGGER_PIN = 5;              // Pin for Trig
const int ECHO_PIN = 6;                 // Pin for Echo
const float MAX_VOLUME_CM3 = 2258211.45; // Tank capacity in cubic centimeters
const float AREA_MULTIPLIER = 14379.944; // Area factor for volume calculation
const int VOLUME_THRESHOLD_CM3 = 1000000; // Minimum acceptable volume

// LCD settings
LiquidCrystal_I2C lcd(0x27, 16, 4); // I2C address 0x27, 16 columns x 4 rows

// Ultrasonic sensor object
Ultrasonic ultrasonic(TRIGGER_PIN, ECHO_PIN);

// =============================
// SETUP
// =============================
void setup() {
  Serial.begin(9600);

  lcd.init();
  lcd.backlight();
  lcd.clear();

  displayWelcomeMessage();
}

// =============================
// MAIN LOOP
// =============================
void loop() {
  int distance = ultrasonic.read(); // in centimeters

  // Calculate water volume
  float currentVolume = MAX_VOLUME_CM3 - distance * AREA_MULTIPLIER;
  float percentFull = (currentVolume / MAX_VOLUME_CM3) * 100 + 10;

  if (currentVolume >= 0) {
    Serial.print("Water Level: ");
    Serial.print(percentFull, 0);
    Serial.println("%");

    // Decide water level status
    String status = (currentVolume >= VOLUME_THRESHOLD_CM3) ?
                    "Water is enough" :
                    "Water is low";

    displayStatus(percentFull, status);
  }

  delay(1000); // 1-second delay
}

// =============================
// HELPER FUNCTIONS
// =============================

void displayWelcomeMessage() {
  lcd.setCursor(0, 0);
  lcd.print("Blue Francis");
  lcd.setCursor(0, 1);
  lcd.print("Water Level Monitor");
  delay(3000);
  lcd.clear();
}

void displayStatus(float percentage, String message) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Level: ");
  lcd.print(percentage, 0);
  lcd.print("%");

  lcd.setCursor(0, 2);
  lcd.print("Status: ");
  lcd.setCursor(0, 3);
  lcd.print(message);
}
