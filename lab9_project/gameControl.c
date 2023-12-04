

#include "gameControl.h"
#include "ball.h"
#include "paddle.h"
#include <stdio.h>

#define BACKGROUND_COLOR DISPLAY_BLACK

// Initialize the game control logic
// This function will initialize everything else
void gameControl_init() {
  display_fillScreen(BACKGROUND_COLOR);

  ball_init();
  paddle_init();
}

// Tick the game control logic
void gameControl_tick() {
    ball_init();
    paddle_tick();
}
