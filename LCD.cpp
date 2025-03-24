#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Set the LCD I2C address to 0x27 and specify 16 columns and 2 rows
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  lcd.init();         // Initialize the LCD
  lcd.backlight();    // Turn on the LCD backlight
  
  lcd.setCursor(0, 0); // Set cursor to first row, first column
  lcd.print("Hello, ESP32!");  // Display text on the first line
  
  lcd.setCursor(0, 1); // Set cursor to second row, first column
  lcd.print("I2C LCD Test :)"); // Display text on the second line
}

void loop() {
  // Nothing here for now, as this is just a static text test
}
