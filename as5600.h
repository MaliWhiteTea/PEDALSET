#ifndef AS5600_H
#define AS5600_H

#include <Arduino.h>

namespace AS5600 {

void init();
bool readSteeringAxis(int &outAxis1023);

}  // namespace AS5600

#endif
