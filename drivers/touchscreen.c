#include "touchscreen.h"
#include "display.h"
#include <stdio.h>

// defined time: 50ms for time to wait before counrted as pressed
#define WAITTIME .05

// global variables
static uint32_t ADC_TIMER = 0;
static double ADC_SETTLE_TICKS = 0;
static bool pressed;
static uint8_t zVar;

// Status of the touchscreen
static touchscreen_status_t touchStatus;

// Variable for finding current point
display_point_t myPoint;

// Touchscreen states
enum state_control_t { WAITING, PRESSED_ST, ADC_SETTLING };
// Variable for switching states
static enum state_control_t currentState;

// Initialize the touchscreen driver state machine, with a given tick period (in
// seconds).
void touchscreen_init(double period_seconds) {
  ADC_SETTLE_TICKS = WAITTIME / period_seconds;
  ADC_TIMER = 0;
  currentState = WAITING;
  touchStatus = TOUCHSCREEN_IDLE;
}

// State degug function
void debugStatePrint() {
  static enum state_control_t previousState;
  static bool firstPass = true;

  // Only print the message if:
  // 1. This the first pass and the value for previousState is unknown.
  // 2. previousState != currentState - this prevents reprinting the same state
  // name over and over.
  if (previousState != currentState || firstPass) {
    firstPass = false; // previousState will be defined, firstPass is false.
    previousState =
        currentState;       // keep track of the last state that you were in.
    switch (currentState) { // This prints messages based upon the state that
                            // you were in.
    case WAITING:
      // print waiting
      //printf("Waiting\n");
      break;
    case ADC_SETTLING:
      // print settling
      //printf("Settling\n");
      break;
    case PRESSED_ST:
      // print pressed
      //printf("Pressed\n");
      break;
    }
  }
}

// Tick the touchscreen driver state machine
void touchscreen_tick() {
  debugStatePrint();
  // Update states
  switch (currentState) {
  case WAITING:
    // switch to ADC_SETTLING
    if (display_isTouched()) {
      currentState = ADC_SETTLING;
      // clear data before you draw again
      display_clearOldTouchData();
    }
    break;
  case ADC_SETTLING:
    // switch to WAITING
    if (!display_isTouched()) {
      currentState = WAITING;
      touchStatus = TOUCHSCREEN_IDLE;
    } else if (display_isTouched() && (ADC_TIMER == ADC_SETTLE_TICKS)) {
      // move on to the Pressed state and update the current touch point
      currentState = PRESSED_ST;
      touchStatus = TOUCHSCREEN_PRESSED;
      display_getTouchedPoint(&myPoint.x, &myPoint.y, &zVar);
      touchscreen_get_location();
    }
    break;
  case PRESSED_ST:
    // go back to waiting if the display is not touched
    if (!display_isTouched()) {
      currentState = WAITING;
      touchStatus = TOUCHSCREEN_RELEASED;
    }
    break;
  }

  // Action states
  switch (currentState) {
  case WAITING:
    // set adc_timer to 0
    ADC_TIMER = 0;
    break;
  case ADC_SETTLING:
    // increment adc_timer and get the touched point
    ADC_TIMER++;
    break;
  case PRESSED_ST:
    // make pressed true while screen is touched
    pressed = true;
    break;
  }
}

// Return the current status of the touchscreen
touchscreen_status_t touchscreen_get_status() { return touchStatus; }

// Acknowledge the touchscreen touch.  This function will only have effect when
// the touchscreen is in the TOUCHSCREEN_RELEASED status, and will cause it to
// switch to the TOUCHSCREEN_IDLE status.
void touchscreen_ack_touch() {
  // When touchscreen is released, make status idle and make pressed false
  if (touchStatus == TOUCHSCREEN_RELEASED) {
    touchStatus = TOUCHSCREEN_IDLE;
    pressed = false;
  }
}

// Get the (x,y) location of the last touchscreen touch
display_point_t touchscreen_get_location() { return myPoint; }
