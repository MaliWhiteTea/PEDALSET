#ifndef AS5600_H
#define AS5600_H

#include <Arduino.h>
#include <stdint.h>

namespace AS5600 {

constexpr uint8_t I2C_ADDRESS = 0x36;
constexpr uint8_t REG_RAW_ANGLE = 0x0C;

void init();
bool readRawAngle(uint16_t &out);

}  // namespace AS5600

#endif
