#include "geo.hh"

double earthRadius(double lat)
{
    // Convert latitude to rad
    double lat_rad = lat * DEG2RAD;

    double An = WGS84_a * WGS84_a * cos(lat_rad);
    double Bn = WGS84_b * WGS84_b * sin(lat_rad);
    double Ad = WGS84_a * cos(lat_rad);
    double Bd = WGS84_b * sin(lat_rad);

    return double(sqrt( (An*An + Bn*Bn) / (Ad*Ad + Bd*Bd) ) / (double)1000);
}

coord getPointAtDistance(coord center, double distance, double bearing)
{
    // Result struct
    coord endCoords;

    // Convert decimal degress to radians
    double start_lat_rad = center.lat * DEG2RAD;
    double start_lon_rad = center.lon * DEG2RAD;
    double bearing_rad = bearing * DEG2RAD;

    // Calclate the ratio of distance to earth's radius (used in the following equations)
    double dR = distance / earthRadius(center.lat);

    // Calculate resulting lat/lon using trig
    double end_lat_rad = asin( sin(start_lat_rad) * cos(dR) + cos(start_lat_rad) * sin(dR) * cos(bearing_rad) );
    double end_lon_rad = start_lon_rad + atan2( 
        sin(bearing_rad) * sin(dR) * cos(start_lat_rad),
        cos(dR) - sin(start_lat_rad) * sin(end_lat_rad) 
    );

    endCoords.lat = end_lat_rad / DEG2RAD;
    endCoords.lon = end_lon_rad / DEG2RAD;

    return endCoords;
}

bbox getCircularBoundingBox(coord center, double radius)
{
    // Result bbox
    bbox result;

    // Convert input degrees to rads
    double lat_rad = center.lat * DEG2RAD;
    double lon_rad = center.lon * DEG2RAD;

    // Get earth's radius at the specified latitude (km)
    double e_rad = earthRadius(center.lat);

    // Get parallel radius at latitude (km)
    double p_rad = e_rad * cos(lat_rad);

    // Calculate bounds (radians)
    double latMin = lat_rad - (radius / e_rad);
    double latMax = lat_rad + (radius / e_rad);
    double lonMin = lon_rad - (radius / p_rad);
    double lonMax = lon_rad + (radius / p_rad);
    
    result.lower_right = { latMin / DEG2RAD, lonMin / DEG2RAD };
    result.upper_left = { latMax / DEG2RAD, lonMax / DEG2RAD };

    return result;
}