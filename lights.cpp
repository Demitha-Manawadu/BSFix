#include <Wire.h> 
#include <BH1750.h>

BH1750 lightMeter;

const int mosfetPin = 23; // GPIO pin connected to the MOSFET Gate
const int thresholdLux = 200; // Light intensity threshold in lux

void setup() {
  Serial.begin(9600);         // Initialize serial communication
  Wire.begin();               // Start I2C communication
  lightMeter.begin();         // Initialize BH1750 light sensor
  pinMode(mosfetPin, OUTPUT); // Set MOSFET pin as output
  Serial.println("GY-30 BH1750 Light Sensor Test");
}

void loop() {
  float lux = lightMeter.readLightLevel(); // Read light intensity in lux
  Serial.print("Light Intensity: ");
  Serial.print(lux);
  Serial.println(" lx");

  if (lux < thresholdLux) {
    analogWrite(mosfetPin, 128); // Set brightness to 50% (128/255)
    Serial.println("Light turned ON (low brightness)");
  } else {
    analogWrite(mosfetPin, 0); // Turn off the light
    Serial.println("Light turned OFF (sufficient brightness)");
  }

  delay(1000); // Read light and adjust every second
}
