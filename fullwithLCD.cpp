#include <DHT.h>    // Include DHT sensor library
#include <Wire.h>   // Include Wire library for I2C
#include <BH1750.h> // Include BH1750 light sensor library
#include <LiquidCrystal_I2C.h> // Include I2C LCD library

// DHT Sensor Setup
#define DHT_PIN 4         // Pin connected to DHT sensor
#define DHT_TYPE DHT22    // DHT22 sensor model
#define CONTROL_PIN 19    // ESP GPIO pin connected to the Mist Maker (MOSFET gate)
DHT dht(DHT_PIN, DHT_TYPE);

// Light Sensor Setup
BH1750 lightMeter;
const int mosfetPin = 23;     // GPIO pin connected to the MOSFET Gate for light
const int thresholdLux = 200; // Light intensity threshold in lux

// LCD Setup
LiquidCrystal_I2C lcd(0x27, 16, 2);  // 0x27 is the I2C address, 16x2 LCD

// Variables
bool mistMakerState = false;       // Tracks Mist Maker state (OFF initially)
unsigned long previousMillis = 0;  // Timer for non-blocking delay
const long interval = 5000;        // 5 seconds delay between readings

void setup() {
  // Mist Maker Setup
  pinMode(CONTROL_PIN, OUTPUT);  // Set Mist Maker pin as output
  digitalWrite(CONTROL_PIN, LOW);  // Start with Mist Maker OFF
  
  // Light Setup
  pinMode(mosfetPin, OUTPUT);  // Set MOSFET pin for light as output

  // Sensor Initialization
  Serial.begin(9600);         // Start serial communication
  Wire.begin();               // Start I2C communication
  dht.begin();                // Initialize DHT sensor
  lightMeter.begin();         // Initialize BH1750 light sensor

  // LCD Initialization
  lcd.init();                 // Initialize the LCD
  lcd.backlight();            // Turn on LCD backlight

  // Welcome Messages
  Serial.println("System Initialized. Monitoring Temperature, Humidity, and Light Intensity...");
  Serial.println("Type '1' in the Serial Monitor to toggle the Mist Maker ON/OFF manually.");
  lcd.setCursor(0, 0);
  lcd.print("System Initialized");
  lcd.setCursor(0, 1);
  lcd.print("Monitoring Sensors");
  delay(2000);  // Show initialization message for 2 seconds
}

void loop() {
  unsigned long currentMillis = millis();

  // Read and Display Temperature and Humidity every 5 seconds
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;  // Reset timer

    float humidity = dht.readHumidity();
    float temperature = dht.readTemperature();  // Temperature in Celsius
    float lux = lightMeter.readLightLevel();    // Read light intensity in lux

    // Check if sensor readings are valid
    if (isnan(humidity) || isnan(temperature) || isnan(lux)) {
      Serial.println("Failed to read from sensors!");
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Sensor Error!");
    } else {
      // Display sensor readings on Serial Monitor
      Serial.print("Temperature: ");
      Serial.print(temperature);
      Serial.print(" °C\t");
      Serial.print("Humidity: ");
      Serial.print(humidity);
      Serial.print(" %\t");
      Serial.print("Light Intensity: ");
      Serial.print(lux);
      Serial.println(" lx");

      // Display sensor readings on LCD
      lcd.clear();  // Clear the previous readings
      lcd.setCursor(0, 0);
      lcd.print("Temp: ");
      lcd.print(temperature);
      lcd.print("C");
      lcd.setCursor(0, 1);
      lcd.print("Hum: ");
      lcd.print(humidity);
      lcd.print("%");

      delay(1500);  // Show Temp & Hum for 1.5 sec

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Light: ");
      lcd.print(lux);
      lcd.print(" lx");

      // If humidity is below threshold, show a message prompting the user to turn on the Mist Maker
      if (humidity < 90) {
        Serial.println("Humidity is low! Turn on the Mist Maker.");
        lcd.setCursor(0, 1);
        lcd.print("Low Humidity!");
      } else {
        Serial.println("Humidity is sufficient.");
        lcd.setCursor(0, 1);
        lcd.print("Humidity OK!");
      }

      delay(1500);  // Show light & humidity message for 1.5 sec

      // Adjust light brightness based on light intensity
      if (lux < thresholdLux) {
        analogWrite(mosfetPin, 128); // Set brightness to 50% (128/255)
        Serial.println("Light turned ON (low brightness)");
      } else {
        analogWrite(mosfetPin, 0); // Turn off the light
        Serial.println("Light turned OFF (sufficient brightness)");
      }
    }
  }

  // Manual Mist Maker Control via Serial Monitor
  if (Serial.available() > 0) {
    char input = Serial.read();  // Read input from Serial Monitor

    if (input == '1') {
      mistMakerState = !mistMakerState;  // Toggle the Mist Maker state

      if (mistMakerState) {
        digitalWrite(CONTROL_PIN, HIGH);  // Turn ON Mist Maker
        Serial.println("Mist Maker ON (Manual)");
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Mist Maker: ON");
      } else {
        digitalWrite(CONTROL_PIN, LOW);   // Turn OFF Mist Maker
        Serial.println("Mist Maker OFF (Manual)");
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Mist Maker: OFF");
      }
      delay(2000);  // Display Mist Maker status for 2 seconds
    }
  }
}
