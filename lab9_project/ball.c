

#include "ball.h"
#include "math.h"
#include "paddle.h"
#include <stdio.h>

#define PI 3.14159265

#define BALL_BASE_SPEED (50 * .045)
#define TICKS_TO_WAIT_ON_PADDLE 20
#define TICKS_TO_WAIT_WHILE_DEAD 500
#define BALL_COLOR DISPLAY_WHITE
#define BACKGROUND_COLOR DISPLAY_BLACK

void drawBall();
void eraseBall();

// Used to track the current x,y
int16_t x_current;
int16_t y_current;

// Used to track the current speed
double_t speed;

// Used to track the current angle (0deg being straight right)
double_t angle;
double_t prevAngle; // used to reduce math calculations

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
display_point_t paddleLocation;

uint16_t tickCount;
int16_t x_change;
int16_t y_change;

void ball_init() { currentState = init_st; }

void ball_tick() {
  switch (currentState) {
  case init_st:
    currentState = on_paddle_st;
    tickCount = 0;
    dead = false;
    angle = PI / 2;
    prevAngle = 0;
    speed = BALL_BASE_SPEED;
    x_current = DISPLAY_WIDTH / 2;
    y_current = 220 - BALL_RADIUS;
    drawBall();
    break;

  case on_paddle_st:
    eraseBall();
    paddleLocation = paddle_getXY();
    x_current = paddleLocation.x;
    y_current = paddleLocation.y - BALL_RADIUS;
    drawBall();

    // after waiting the time needed, start moving
    if (TICKS_TO_WAIT_ON_PADDLE == tickCount++) {
      tickCount = 0;
      currentState = moving_st;
    }
    break;

  case moving_st:
    // calculate angles
    if (angle != prevAngle) {
      x_change = (round)(cos(angle) * speed);
      y_change = (round)(sin(angle) * speed);
      // printf("angle: %f\nx: %d\ny: %d\nsin: %f\n", angle, x_change, y_change,
      // sin(angle));
      prevAngle = angle;
    }

    // erase ball
    eraseBall();

    y_current -= y_change;
    x_current += x_change;

    if (y_current > DISPLAY_HEIGHT - BALL_RADIUS) {
      // ball hit ground
      y_current = DISPLAY_HEIGHT - BALL_RADIUS;
      currentState = dead_st;
      dead = true;
    }

    if (y_current < BALL_RADIUS) {
      // hit the top
      y_current = BALL_RADIUS;
      angle = 2 * PI - angle;
    }

    if (x_current < BALL_RADIUS) {
      // hit the left
      x_current = BALL_RADIUS;
      angle = PI - angle;
    }

    if (x_current > DISPLAY_WIDTH - BALL_RADIUS) {
      // hit the right
      x_current = DISPLAY_WIDTH - BALL_RADIUS;
      angle = PI - angle;
    }

    // check to keep angle between 0 and 2PI
    if (angle > 2 * PI)
      angle -= 2 * PI;
    else if (angle < 0)
      angle += 2 * PI;

    // redrawBall
    drawBall();
    break;

  case dead_st:
    if (TICKS_TO_WAIT_WHILE_DEAD == tickCount++) {
      tickCount = 0;
      currentState = on_paddle_st;
      dead = false;
      angle = PI / 2;
    }
    break;
  }
}

bool ball_hit_ground() { return dead; }

void drawBall() {
  display_fillCircle(x_current, y_current, BALL_RADIUS, BALL_COLOR);
}

void eraseBall() {
  display_fillCircle(x_current, y_current, BALL_RADIUS, BACKGROUND_COLOR);
}