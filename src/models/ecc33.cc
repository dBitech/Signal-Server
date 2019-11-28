#include <cmath>

double ECC33pathLoss(float f, float TxH, float RxH, float d, int mode) {
  // Sanity check as this model operates within limited Txh/Rxh bounds
  if (TxH - RxH < 0) {
    RxH = RxH / (d * 2);
  }

  // MHz to GHz
  f = f / 1000;

  double Gr = 0.759 * RxH - 1.862;  // Big city with tall buildings (1)
  // PL = Afs + Abm - Gb - Gr
  double Afs = 92.4 + 20 * std::log10(d) + 20 * std::log10(f);
  double Abm = 20.41 + 9.83 * std::log10(d) + 7.894 * std::log10(f) + 9.56 * (std::log10(f) * std::log10(f));
  double Gb = std::log10(TxH / 200) * (13.958 + 5.8 * (std::log10(d) * std::log10(d)));
  if (mode > 1) {  // Medium city (Europe)
    Gr = (42.57 + 13.7 * std::log10(f)) * (std::log10(RxH) - 0.585);
  }

  return Afs + Abm - Gb - Gr;
}
