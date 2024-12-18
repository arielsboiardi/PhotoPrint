#include <Arduino.h>
#include <Wire.h>

#define I2C_ADDRESS 0x1A

void setup() {
  Wire.begin(); // I2C0 


}

void loop() {

  Wire.beginTransmission(I2C_ADDRESS); // begin transmit to device 1
  Wire.write(0x90); // register 0x10 with bit 7 set to 1 for write mode
  // Wire.write(0x04); // turn on blue led
  Wire.write(0x00); // turn off blue led
  Wire.endTransmission(); // end writing without sending stop bit

  delay(100);
}