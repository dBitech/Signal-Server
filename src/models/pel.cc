/*****************************************************************************
 *  Plane Earth Path Loss model for Signal Server by Alex Farrant             *
 *  Taken from "Antennas and Propagation for wireless communication systems"  *
 *  ISBN 978-0-470-84879-1                                                    *
 *  10 August 2016							     *
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

/*
Plane Earth Loss model
Frequency: N/A
Distance (km): Any
*/
double PlaneEarthLoss(float d, float TxH, float RxH) {
  // Plane earth loss is independent of frequency.
  double dbloss = 40 * std::log10(d) + 20 * std::log10(TxH) + 20 * std::log10(RxH);
  return dbloss;
}
