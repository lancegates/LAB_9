

#include "ball.h"
#include <stdio.h>

#define BALL_BASE_SPEED 30
#define BALL_COLOR DISPLAY_WHITE
#define BACKGROUND_COLOR DISPLAY_BLACK

#define DEG_90 90
#define DEG_180 190

void drawBall();
void eraseBall();

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
typedef enum {
  init_st,      // Start here, transition out of this state on the first tick.
  on_paddle_st, // Go here when ball needs to be respawned(whiole it is sitting
                // on the paddle)
  moving_st,    // In this state while it's moving
  dead_st, // here if the ball is dead (after it hits the ground or you won)
} plane_st_t;

plane_st_t currentState;

void ball_init() { currentState = init_st; }

void ball_tick() {
  switch (currentState) {
  case init_st:
    currentState = on_paddle_st;
    dead = false;
    angle = DEG_90;
    break;

  case on_paddle_st:
    /* code */
    break;

  case moving_st:
    /* code */
    break;

  case dead_st:
    /* code */
    break;
  }
}

bool ball_hit_ground() {}

void drawBall() {
  display_fillCircle(x_current, y_current, BALL_RADIUS, BALL_COLOR);
}

void eraseBall() {
  display_fillCircle(x_current, y_current, BALL_RADIUS, BACKGROUND_COLOR);
}