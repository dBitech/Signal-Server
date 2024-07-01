#ifndef _LOS_HH_
#define _LOS_HH_

#include <stdio.h>
#include <stdint.h>

#include "../common.hh"

// Propagation models supported
enum PropModel {
    ITM_P2P = 0,
    ITM_LR = 1,
    LOS = 2,
    HATA = 3,
    ECC33 = 4,
    SUI = 5,
    COST231_HATA = 6,
    ITU_R = 7,
    ITWOM_3 = 8,
    ERICSSON = 9,
    PLANE_EARTH = 10,
    ELGI_V_U = 11,
    SOIL = 12,
};

// Rectangular bounding box propagation range
struct PropagationRange {
    double min_west, max_west, min_north, max_north;
    double altitude;
    bool eastwest, los, use_threads;
    site source;
    unsigned char mask_value;
    FILE *fd;
    PropModel prop_model;
    int knifeedge, pmenv;
};

// Angular propagation area
struct PropagationRadius {
    double start_angle_rad, stop_angle_rad;
    double radius;
    double altitude;
    bool los, use_threads;
    site source;
    unsigned char mask_value;
    FILE *fd;
    PropModel prop_model;
    int knifeedge, pmenv, points;
};

void PlotLOSPath(struct site source, struct site destination, char mask_value);

void PlotPropPath(struct site source, struct site destination, unsigned char mask_value, FILE *fd, PropModel propmodel, int knifeedge,
                  int pmenv);

void PlotLOSMap(struct site source, double altitude, char *plo_filename, bool use_threads, uint8_t segments);

void PlotPropagation(struct site source, bbox bounds, 
                    double altitude, char *plo_filename,
		            PropModel propmodel, int knifeedge, int haf, int pmenv, 
                    bool use_threads, uint8_t segments);

/// @brief Plot propagation using a center point and circular radius. This plots around a circle instead of a rectangular bounding box and is theoretically more efficient.
/// @param source source transmitter
/// @param range maximum plot rage in miles or km
/// @param altitude altitude in ft or m
/// @param plot_filename output plot filename
/// @param prop_model propagation model to use
/// @param use_threads whether to use multithreading
/// @param segments segments to split the plot circle into (must be a multiple of 2 or 3)
void PlotPropagationRadius(struct site source, double range, 
                            double altitude, char *plot_filename, 
                            PropModel prop_model, int knifeedge, int haf, int pmenv, 
                            bool use_threads, uint8_t segments);

void PlotPath(struct site source, struct site destination, char mask_value);

#endif /* _LOS_HH_ */
