#include <cmath>

double EricssonpathLoss(float f, float TxH, float RxH, float d, int mode) {
  /*
     AKA Ericsson 9999 model
   */
  // Urban
  double a0 = 36.2, a1 = 30.2, a2 = -12, a3 = 0.1;

  if (mode == 2) {  // Suburban / Med loss
    a0 = 43.2;
    a1 = 68.93;
  }
  if (mode == 1) {  // Rural
    a0 = 45.95;
    a1 = 100.6;
  }
  double g1 = 3.2 * (std::log10(11.75 * RxH) * std::log10(11.75 * RxH));
  double g2 = 44.49 * std::log10(f) - 4.78 * (std::log10(f) * std::log10(f));

  return a0 + a1 * std::log10(d) + a2 * std::log10(TxH) + a3 * std::log10(TxH) * std::log10(d) - g1 + g2;
}
