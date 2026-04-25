#include "pedals.h"

namespace {

constexpr int PIN_GAS = A0;
constexpr int PIN_BRAKE = A1;
constexpr int PIN_CLUTCH = A2;

// Genel pedal yönü ters ise true yap.
constexpr bool kInvertPedals = false;
// true: fren ilk harekette yavas, sonlara dogru daha agresif artar.
constexpr bool realisticBrake = true;
// A0 pedal kalibrasyonu: 604 -> 0, 270 -> 1023
constexpr int A0_RAW_MIN = 270;
constexpr int A0_RAW_MAX = 604;
// Fren (A1) kalibrasyonu: 614 -> 0, 244 -> 1023
constexpr int BRAKE_RAW_MIN = 244;
constexpr int BRAKE_RAW_MAX = 603;
constexpr int BRAKE_SPLIT_RAW = 450;
constexpr int BRAKE_SPLIT_OUTPUT = 120;

static int toPedalAxis(int raw) {
  int v = kInvertPedals ? (1023 - raw) : raw;
  return constrain(v, 0, 1023);
}

static int mapBrakeAxis(int raw) {
  if (realisticBrake) {
    long mapped;
    if (raw >= BRAKE_SPLIT_RAW) {
      // 603..450 arasi: az artis
      mapped = map(raw, BRAKE_RAW_MAX, BRAKE_SPLIT_RAW, 0, BRAKE_SPLIT_OUTPUT);
    } else {
      // 450..244 arasi: daha hizli artis
      mapped = map(raw, BRAKE_SPLIT_RAW, BRAKE_RAW_MIN, BRAKE_SPLIT_OUTPUT, 1023);
    }
    return constrain(static_cast<int>(mapped), 0, 1023);
  }

  long mapped = map(raw, BRAKE_RAW_MAX, BRAKE_RAW_MIN, 0, 1023);
  return constrain(static_cast<int>(mapped), 0, 1023);
}

static int mapGasAxis(int raw) {
  long mapped = map(raw, A0_RAW_MAX, A0_RAW_MIN, 0, 1023);
  return constrain(static_cast<int>(mapped), 0, 1023);
}

}  // namespace

void Pedals::init() {
  pinMode(PIN_GAS, INPUT);
  pinMode(PIN_BRAKE, INPUT);
  pinMode(PIN_CLUTCH, INPUT);
}

void Pedals::read(Values &out) {
  out.gas = mapGasAxis(analogRead(PIN_GAS));
  out.brake = mapBrakeAxis(analogRead(PIN_BRAKE));
  out.clutch = toPedalAxis(analogRead(PIN_CLUTCH));
}
