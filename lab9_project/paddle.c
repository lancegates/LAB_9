#include "paddle.h"
#include "ball.h"
#include "display.h"
#include "buttons.h"

//right/left edge of screen 
#define RIGHT_EDGE 320
#define LEFT_EDGE 0

//paddle speed
#define PADDLE_SPEED 4

//middle of screen
#define START_LOCATION_X 160

//bottom part of screen
#define LOCATION_Y 220

//paddle dimensions
#define PADDLE_LENGTH 50
#define PADDLE_WIDTH 5

//paddle current position in x
uint16_t currentPosition;

//bool for telling if ball hit paddle
bool isHit;

// Paddle states
enum paddle_control_t { INITIAL, GO_RIGHT, GO_LEFT, STOP };
// Variable for switching states
static enum paddle_control_t currentState;

void drawPaddle() {
    display_fillRect(currentPosition-(PADDLE_LENGTH/2), LOCATION_Y,
     PADDLE_LENGTH, PADDLE_WIDTH, DISPLAY_WHITE);
}

void erasePaddle() {
    display_fillRect(currentPosition-(PADDLE_LENGTH/2), LOCATION_Y,
     PADDLE_LENGTH, PADDLE_WIDTH, DISPLAY_BLACK);
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
    buttons_init();
    currentState = STOP;
    break;

  case STOP:
    if ((buttons_read() & BUTTONS_BTN0_MASK) == BUTTONS_BTN0_MASK) {
        currentState = GO_RIGHT;
        
    }
    else if ((buttons_read() & BUTTONS_BTN3_MASK) == BUTTONS_BTN3_MASK) {
        currentState = GO_LEFT;
        
    }
    break;

  case GO_RIGHT:
    if ((buttons_read() & BUTTONS_BTN0_MASK) != BUTTONS_BTN0_MASK) {
        currentState = STOP;
    }
    break;

  case GO_LEFT:
    if ((buttons_read() & BUTTONS_BTN3_MASK) != BUTTONS_BTN3_MASK) {
        currentState = STOP;
    }
    break;
  }

  // Action state machine
  switch (currentState) {
  case INITIAL:
    
    break;
  
  case STOP:
    drawPaddle();
    break;

  case GO_RIGHT:
    //Check if you reached the right edge
    if (currentPosition + (PADDLE_LENGTH/2) < RIGHT_EDGE) {
        erasePaddle();
        currentPosition = currentPosition + PADDLE_SPEED;
        drawPaddle();
    }
    break;

  case GO_LEFT:
    //Check if you reached the left edge
    if (currentPosition - (PADDLE_LENGTH/2) > LEFT_EDGE) {
        erasePaddle();
        currentPosition = currentPosition - PADDLE_SPEED;
        drawPaddle();
    }
    break;
  }
}

//return the angle the ball should go
void paddle_get_angle();

//return if ball hit then set to false
bool paddle_is_hit(); 

// Get the XY location of the paddle
display_point_t paddle_getXY() {
    display_point_t myPoint;
    myPoint.x = currentPosition;
    myPoint.y = LOCATION_Y;
    return myPoint;
}