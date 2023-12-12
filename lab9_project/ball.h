#ifndef BALL
#define BALL

#include "display.h"
#include <stdbool.h>
#include <stdint.h>

#define BALL_RADIUS 5

void ball_init();

void reset_ball();

void ball_tick();

bool ball_hit_ground();

display_point_t ball_getXY();

// maybe  speed and angle as variable in here if we want to change it from other
// locations

#endif /*BALL*/