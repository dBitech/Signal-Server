/**
 * @file geo.hh
 * @ingroup geo
 * @file geo.cc
 * @ingroup geo
 * 
*/

#ifndef __GEO_HH_
#define __GEO_HH_

#include <stdio.h>
#include <stdint.h>
#include <math.h>

#include "common.hh"

/// WGS84 semi-axes
#define WGS84_a 6378137.0
#define WGS84_b 6356752.3

/// @brief Calculate the approximate radius of the earth at a given latitude, using the WGS84 model
/// @cite http://en.wikipedia.org/wiki/Earth_radius
/// @param lat latitude in degrees
/// @return earth radius in km
double earthRadius(double lat);

/// @brief Get the lat & lon of a point at a certain distance and heading from a given point
/// @cite Adapted from https://www.movable-type.co.uk/scripts/latlong.html and https://stackoverflow.com/a/7835325
/// @param start_lat starting latitude in degrees
/// @param start_lon starting longitude in degrees
/// @param distance distance in km
/// @param bearing bearing in degrees
/// @return coodinates of the resultant point in decimal degrees
coord getPointAtDistance(coord center, double distance, double bearing);

/// @brief Get the bounding box for a circle at a given lat/lon and radius
/// @param center center coordinates in decimal degrees
/// @param radius radius in km
/// @return bounding box
bbox getCircularBoundingBox(coord center, double radius);

#endif