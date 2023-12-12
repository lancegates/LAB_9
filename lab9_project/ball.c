

#include "ball.h"
#include "gameControl.h"
#include "math.h"
#include "paddle.h"
#include "tile.h"
#include <stdio.h>

#define PI 3.14159265

#define BALL_BASE_SPEED 3
#define BALL_SPEED_INCREASE_BY .125
#define TICKS_TO_WAIT_ON_PADDLE 20
#define TICKS_TO_WAIT_WHILE_DEAD 10
#define BALL_COLOR DISPLAY_WHITE
#define BACKGROUND_COLOR DISPLAY_BLACK
#define TILE_WIDTH 29
#define TILE_HIEGHT 15
#define PADDLE_LENGTH 50
#define PADDLE_HIEGHT 5

void drawBall();
void eraseBall();
void tryBounceOffPaddle();
void tryBounceOffTile();

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
uint16_t padEdge;

uint16_t tickCount;
int16_t x_change;
int16_t y_change;

gameTiles_t *gameTiles;
void ball_init(gameTiles_t *initTile) {
  currentState = init_st;
  gameTiles = initTile;
}

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
      // hit the left wall
      x_current = BALL_RADIUS;
      angle = PI - angle;
    }

    if (x_current > DISPLAY_WIDTH - BALL_RADIUS) {
      // hit the right wall
      x_current = DISPLAY_WIDTH - BALL_RADIUS;
      angle = PI - angle;
    }

    tryBounceOffPaddle();
    tryBounceOffTile();

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

void reset_ball() {
  eraseBall();
  currentState = on_paddle_st;
}

bool ball_hit_ground() {
  bool temp = dead;
  dead = false;
  return temp;
}

void drawBall() {
  display_fillCircle(x_current, y_current, BALL_RADIUS, BALL_COLOR);
}

void eraseBall() {
  display_fillCircle(x_current, y_current, BALL_RADIUS, BACKGROUND_COLOR);
}

void tryBounceOffPaddle() {
  paddleLocation = paddle_getXY();

  // first chect to see if ball hit top of paddle
  if (y_current >= paddleLocation.y - BALL_RADIUS &&
      y_current <= paddleLocation.y + PADDLE_HIEGHT + BALL_RADIUS) {
    if (x_current < paddleLocation.x + PADDLE_LENGTH / 2 &&
        x_current > paddleLocation.x - PADDLE_LENGTH / 2) {

      padEdge = paddleLocation.x + PADDLE_LENGTH / 2;
      angle = PI * ((double)(padEdge - x_current) / (double)PADDLE_LENGTH);
    }
  }
  // then check to see if ball hit side of paddle
  else if (y_current > paddleLocation.y) {
  }
}

void tryBounceOffTile() {
  // loop through all tiles
  for (uint16_t i = 0; i < TOTAL_NUM_TILES; i++) {

    if (gameTiles->tile[i].is_dead) {
      continue;
    }

    // check to see if ball has a chance of hitting the tile
    if (y_current <=
            gameTiles->tile[i].y_position + TILE_HIEGHT + BALL_RADIUS &&
        y_current >= gameTiles->tile[i].y_position - BALL_RADIUS) {

      // hit bottom or top of tile?
      if (x_current <= gameTiles->tile[i].x_position + TILE_WIDTH &&
          x_current >= gameTiles->tile[i].x_position) {
        angle = 2 * PI - angle;
        speed += BALL_SPEED_INCREASE_BY;
        gameTiles->tile[i].is_dead = true;
        scoreTile(&(gameTiles->tile[i]));
        eraseTile(&(gameTiles->tile[i]));
        return;
      }

      // hit Right of tile?
      if (x_current <=
              gameTiles->tile[i].x_position + TILE_WIDTH + BALL_RADIUS &&
          x_current >= gameTiles->tile[i].x_position + TILE_WIDTH) {
        angle = PI - angle;
        speed += BALL_SPEED_INCREASE_BY;
        gameTiles->tile[i].is_dead = true;
        scoreTile(&(gameTiles->tile[i]));
        eraseTile(&(gameTiles->tile[i]));
        return;
      }

      // hit Left of tile?
      if (x_current <= gameTiles->tile[i].x_position &&
          x_current >= gameTiles->tile[i].x_position - BALL_RADIUS) {
        angle = PI - angle;
        speed += BALL_SPEED_INCREASE_BY;
        gameTiles->tile[i].is_dead = true;
        scoreTile(&(gameTiles->tile[i]));
        eraseTile(&(gameTiles->tile[i]));
        return;
      }
    }
  }
}
