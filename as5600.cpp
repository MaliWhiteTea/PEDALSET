#include "as5600.h"

#include <Wire.h>

namespace AS5600 {
namespace {

constexpr uint8_t kI2cAddress = 0x36;
constexpr uint8_t kRegRawAngle = 0x0C;

// Hafıza değişkenlerimiz (eksi değerlere inebilmek için long)
long onceki_aci = 0;
long tur_sayisi = 0;
long toplam_pozisyon = 0;
bool has_baseline = false;

bool as5600_oku(long &outRaw4095) {
  Wire.beginTransmission(kI2cAddress);
  Wire.write(kRegRawAngle);
  if (Wire.endTransmission(false) != 0) {
    return false;
  }

  uint8_t n = Wire.requestFrom(kI2cAddress, static_cast<uint8_t>(2));
  if (n < 2) {
    return false;
  }

  int highByte = Wire.read();
  int lowByte = Wire.read();
  if (highByte < 0 || lowByte < 0) {
    return false;
  }

  outRaw4095 = (static_cast<long>(highByte) << 8) | static_cast<long>(lowByte);
  return true;
}

}  // namespace

void init() {
  long raw;
  if (as5600_oku(raw)) {
    onceki_aci = raw;
    tur_sayisi = 0;
    toplam_pozisyon = 0;
    has_baseline = true;
  } else {
    // Okuma başarısızsa baseline sonraki başarılı okumada kurulacak.
    has_baseline = false;
  }
}

bool readSteeringAxis(int &outAxis1023) {
  long mevcut_aci;
  if (!as5600_oku(mevcut_aci)) {
    return false;
  }

  if (!has_baseline) {
    onceki_aci = mevcut_aci;
    tur_sayisi = 0;
    toplam_pozisyon = 0;
    has_baseline = true;
  }

  long fark = mevcut_aci - onceki_aci;

  // Tur Atlama (Sıçrama) Algoritması
  if (fark < -2000) {
    tur_sayisi++;  // 4000'lerden 0'a ani düşüş: ileri doğru tur
  } else if (fark > 2000) {
    tur_sayisi--;  // 0'dan 4000'lere ani fırlama: geriye doğru tur
  }

  // Sınırsız toplam pozisyon (1 tam tur = 4096)
  toplam_pozisyon = (tur_sayisi * 4096L) + mevcut_aci;
  onceki_aci = mevcut_aci;

  // 900 derece = 2.5 tur => -5120..+5120 aralığını 0..1023'e map et
  long oyun_ekseni = map(toplam_pozisyon, -5120L, 5120L, 0L, 1023L);
  oyun_ekseni = constrain(oyun_ekseni, 0L, 1023L);

  outAxis1023 = static_cast<int>(oyun_ekseni);
  return true;
}

}  // namespace AS5600
