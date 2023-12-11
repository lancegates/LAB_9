

#include "gameControl.h"
#include "ball.h"
#include "paddle.h"
#include "tile.h"
#include <stdio.h>

#define BACKGROUND_COLOR DISPLAY_BLACK

static gameTiles_t gameTiles;

static uint8_t gameLives = 3;
static uint8_t previousGameLives;
static uint8_t gamePoints = 0;
static uint8_t previousGamePoints;

static tile_t *red_tiles = &(gameTiles.tile[0]);
static tile_t *green_tile = &(gameTiles.tile[TOTAL_NUM_TILES / 3]);
static tile_t *blue_tile = &(gameTiles.tile[TOTAL_NUM_TILES - (TOTAL_NUM_TILES / 3)]);



// Game states
enum state_control_t { INIT, MENU, GAME_ON, GAME_OVER };
// Variable for switching states
static enum state_control_t currentState;

// Initialize the game control logic
// This function will initialize everything else
void gameControl_init() {

  // init the background as black
  display_fillScreen(BACKGROUND_COLOR);

  // init lives in bottom left corner
  display_setTextSize(.5);
  display_setCursor(5, 230);
  display_setTextColor(DISPLAY_WHITE);
  display_println("Lives: ");
  display_setCursor(45, 230);
  display_printlnDecimalInt(gameLives);

  // init points in bottom right corner
  display_setTextSize(.5);
  display_setCursor(270, 230);
  display_setTextColor(DISPLAY_WHITE);
  display_println("Points: ");
  display_setCursor(315, 230);
  display_printlnDecimalInt(gamePoints);


  // init ball
  ball_init(&gameTiles);
  // init paddle
  paddle_init();

  // Give tile.c the number of tiles per row to draw
  editNumTiles(TOTAL_NUM_TILES / 3);

  // fill red tile objects
  for (uint16_t i = 0; i < (TOTAL_NUM_TILES / 3); i++) {
    tile_init_red(&gameTiles.tile[i], i);
  }

  // fill green tile objects
  for (uint16_t i = 0; i < (TOTAL_NUM_TILES / 3); i++) {
    tile_init_green(&gameTiles.tile[i + (TOTAL_NUM_TILES / 3)], i);
  }

  // fill blue tile objects
  for (uint16_t i = 0; i < (TOTAL_NUM_TILES / 3); i++) {
    tile_init_blue(&gameTiles.tile[i + (TOTAL_NUM_TILES - (TOTAL_NUM_TILES / 3))], i);
  }
}

// Tick the game control logic
void gameControl_tick() {
  ball_tick();
  paddle_tick();

  // Check lives
  if (ball_hit_ground()) {
    if (gameLives > 0) {
      previousGameLives = gameLives;
      gameLives--;
    }
  }

  // Update displayed Lives
  // erase
  display_setCursor(5, 230);
  display_setTextColor(DISPLAY_BLACK);
  display_println("Lives: ");
  display_setCursor(45, 230);
  display_printlnDecimalInt(previousGameLives);
  // update
  display_setCursor(5, 230);
  display_setTextColor(DISPLAY_WHITE);
  display_println("Lives: ");
  display_setCursor(45, 230);
  display_printlnDecimalInt(gameLives);

  // Update displayed Points
  // erase
  display_setCursor(270, 230);
  display_setTextColor(DISPLAY_BLACK);
  display_println("Points: ");
  display_setCursor(310, 230);
  display_printlnDecimalInt(previousGamePoints);
  // update
  display_setCursor(270, 230);
  display_setTextColor(DISPLAY_WHITE);
  display_println("Points: ");
  display_setCursor(315, 230);
  display_printlnDecimalInt(gamePoints);
}
