

#include "ball.h"
#include <stdio.h>

#define BALL_BASE_SPEED 30

// Used to track the current x,y
uint16_t x_current;
uint16_t y_current;

// Used to track the current speed
uint16_t speed;

// Used to track the current angle (0deg being straight right)
uint16_t angle;

// used to tell when ball hits the ground
bool dead;

// Make State Machine

void ball_init() {}

void ball_tick() {}

bool ball_hit_ground() {}