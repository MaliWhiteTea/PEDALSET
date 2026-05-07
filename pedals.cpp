#include "pedals.h"

namespace {

constexpr int PIN_GAS = A0;
constexpr int PIN_BRAKE = A1;
constexpr int PIN_CLUTCH = A2;

// Genel pedal yönü ters ise true yap.
constexpr bool kInvertPedals = false;
// true: fren ilk harekette yavas, sonlara dogru daha agresif artar.
constexpr bool realisticBrake = true;
// Gaz (A0) ham okuma araligi: 960 -> 0, 596 -> 1023
constexpr int GAS_RAW_MIN = 596;
constexpr int GAS_RAW_MAX = 960;
// Fren (A1) ham okuma araligi: 710 -> 0, 305 -> 1023
constexpr int BRAKE_RAW_MIN = 305;
constexpr int BRAKE_RAW_MAX = 710;
constexpr int BRAKE_SPLIT_RAW = 537;
constexpr int BRAKE_SPLIT_OUTPUT = 120;
// Debriyaj (A2) ham okuma araligi: 595 -> 0, 265 -> 1023
constexpr int CLUTCH_RAW_MIN = 265;
constexpr int CLUTCH_RAW_MAX = 595;

static int toPedalAxis(int raw) {
  int v = kInvertPedals ? (1023 - raw) : raw;
  return constrain(v, 0, 1023);
}

static int mapBrakeAxis(int raw) {
  raw = toPedalAxis(raw);
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
  raw = toPedalAxis(raw);
  long mapped = map(raw, GAS_RAW_MAX, GAS_RAW_MIN, 0, 1023);
  return constrain(static_cast<int>(mapped), 0, 1023);
}

static int mapClutchAxis(int raw) {
  raw = toPedalAxis(raw);
  long mapped = map(raw, CLUTCH_RAW_MAX, CLUTCH_RAW_MIN, 0, 1023);
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
  out.clutch = mapClutchAxis(analogRead(PIN_CLUTCH));
}
