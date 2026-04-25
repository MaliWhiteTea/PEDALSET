#include <Wire.h>
#include <Joystick.h>

#include "as5600.h"
#include "pedals.h"

// X: direksiyon. Pedallar ayrı HID kullanımları: Accelerator (gaz), Brake (fren),
// Throttle simgesi (debriyaj) — Windows'ta genelde 0'dan başlayan üç ayrı kaydırıcı.
Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID, JOYSTICK_TYPE_GAMEPAD,
  0, 0,
  true,
  false, false, false, false, false,
  false,
  true,
  true,
  true,
  false);

void setup() {
  Wire.begin();
  AS5600::init();
  Pedals::init();
  Serial.begin(115200);

  Joystick.begin();
  Joystick.setXAxisRange(0, 4095);
  Joystick.setThrottleRange(0, 1023);
  Joystick.setAcceleratorRange(0, 1023);
  Joystick.setBrakeRange(0, 1023);
}

void loop() {
  uint16_t steering;
  if (AS5600::readRawAngle(steering)) {
    Joystick.setXAxis(static_cast<int>(steering));
  }

  Pedals::Values p;
  Pedals::read(p);
  Joystick.setThrottle(p.clutch);
  Joystick.setAccelerator(p.gas);
  Joystick.setBrake(p.brake);
  Serial.println(p.brake);

  delay(10);
}
