#ifndef PEDALS_H
#define PEDALS_H

#include <Arduino.h>

namespace Pedals {

struct Values {
  int gas;        // A0
  int brake;      // A1
  int clutch;     // A2
};

void init();
void read(Values &out);

}  // namespace Pedals

#endif
