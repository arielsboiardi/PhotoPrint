#include <Arduino.h>
#include <Wire.h>

#define I2C_ADDRESS 0x1A

void wireEvent(int howMany) {
  Serial.print("Received on I2C: ");
  while (Wire.available()) {
    byte c = Wire.read(); // receive byte as a character
	Serial.print(c, HEX);
	Serial.println("");
  }
}

void setup() {
	Serial.begin(9600);
	
	// Initialize I2C communication
	Serial.println("Initializing I2C communication...");
	Wire.begin(I2C_ADDRESS);
	Wire.onReceive(wireEvent);
}

void loop() {

}