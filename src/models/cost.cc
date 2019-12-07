/*****************************************************************************
 *  COST231-HATA MODEL for Signal Server by Alex Farrant			     *
 *  30 December 2013i							     *
 *  This program is free software; you can redistribute it and/or modify it   *
 *  under the terms of the GNU General Public License as published by the     *
 *  Free Software Foundation; either version 2 of the License or any later    *
 *  version.								     *
 *									     *
 *  This program is distributed in the hope that it will useful, but WITHOUT  *
 *  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or     *
 *  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License     *
 *  for more details.							     *
 *									     */

#include <cmath>

double COST231pathLoss(float f, float TxH, float RxH, float d, int mode) {
  /*
  COST231 extension to HATA model
  Frequency 1500 to 2000MHz
  TxH = Base station height 30 to 200m
  RxH = Mobile station height 1 to 10m
  Distance 1-20km
  modes 1 = URBAN, 2 = SUBURBAN, 3 = OPEN
  http://morse.colorado.edu/~tlen5510/text/classwebch3.html
  */
  /*	if (f < 150 || f > 2000) {
                  fprintf
                      (stderr,"Error: COST231 Hata model frequency range 150-2000MHz\n");
                  exit(EXIT_FAILURE);
          }
  */
  int C = 3;  // 3dB for Urban
  float lRxH = std::log10(11.75 * RxH);
  float C_H = 3.2 * (lRxH * lRxH) - 4.97;  // Large city (conservative)
  float c0 = 69.55;
  float cf = 26.16;
  if (f > 1500) {
    c0 = 46.3;
    cf = 33.9;
  }
  if (mode == 2) {
    C = 0;  // Medium city (average)
    lRxH = std::log10(1.54 * RxH);
    C_H = 8.29 * (lRxH * lRxH) - 1.1;
  }
  if (mode == 3) {
    C = -3;  // Small city (Optimistic)
    C_H = (1.1 * std::log10(f) - 0.7) * RxH - (1.56 * std::log10(f)) + 0.8;
  }
  float logf = std::log10(f);
  double dbloss = c0 + (cf * logf) - (13.82 * std::log10(TxH)) - C_H + (44.9 - 6.55 * std::log10(TxH)) * std::log10(d) + C;
  return dbloss;
}
