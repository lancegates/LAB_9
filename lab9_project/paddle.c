#include "paddle.h"
#include "ball.h"
#include "display.h"
#include "buttons.h"

//middle of screen
#define START_LOCATION_X 160
//bottom part of screen
#define LOCATION_Y 200

#define PADDLE_LENGTH 30
#define PADDLE_WIDTH 5

uint16_t currentPosition;

// Paddle states
enum paddle_control_t { INITIAL, GO_RIGHT, GO_LEFT, STOP };
// Variable for switching states
static enum paddle_control_t currentState;

void drawPaddle() {
    display_fillRect(currentPosition-(PADDLE_LENGTH/2), LOCATION_Y,
     PADDLE_WIDTH, PADDLE_LENGTH, DISPLAY_WHITE);
}

//initialize paddle
void paddle_init() {
    currentPosition = START_LOCATION_X;
}

//tick paddle object
//state machine
void paddle_tick() { 
  // Transition state machine
  switch (currentState) {
  case INITIAL:
    drawPaddle();
    currentState = STOP;
    break;

  case STOP:
    
    break;

  case GO_RIGHT:
    
    break;

  case GO_LEFT:
    
    break;
  }

  // Action state machine
  switch (currentState) {
  case INITIAL:
    
    break;
  
  case STOP:
    break;

  case GO_RIGHT:
    
    break;

  case GO_LEFT:
    
    break;
  }
}

//return the angle the ball should go
void paddle_get_angle();

//return if ball hit then set to false
bool paddle_is_hit(); 

// Get the XY location of the paddle
display_point_t paddle_getXY();