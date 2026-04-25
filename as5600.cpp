#include "as5600.h"
#include <Wire.h>

void AS5600::init() {
  // Wire.begin() ana sketch'te çağrılmalı; burada ek I2C ayarı gerekirse eklenebilir.
}

bool AS5600::readRawAngle(uint16_t &out) {
  Wire.beginTransmission(I2C_ADDRESS);
  Wire.write(REG_RAW_ANGLE);
  if (Wire.endTransmission(false) != 0) {
    return false;
  }
  uint8_t n = Wire.requestFrom(static_cast<uint8_t>(I2C_ADDRESS), static_cast<uint8_t>(2));
  if (n < 2) {
    return false;
  }
  uint16_t highByte = Wire.read();
  uint16_t lowByte = Wire.read();
  out = (highByte << 8) | lowByte;
  return true;
}
