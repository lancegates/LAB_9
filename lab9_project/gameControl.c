

#include "gameControl.h"
#include "ball.h"
#include "paddle.h"
#include "tile.h"
#include <stdio.h>

#define BACKGROUND_COLOR DISPLAY_BLACK

#define TOTAL_NUM_TILES 9

tile_t tile[TOTAL_NUM_TILES];
tile_t *red_tiles = &(tile[0]);
tile_t *green_tile = &(tile[TOTAL_NUM_TILES/3]);
tile_t *blue_tile = &(tile[TOTAL_NUM_TILES - (TOTAL_NUM_TILES/3)]);

// Initialize the game control logic
// This function will initialize everything else
void gameControl_init() {
  display_fillScreen(BACKGROUND_COLOR);

  ball_init();
  paddle_init();

  //Give tile.c the number of tiles per row to draw
  editNumTiles(TOTAL_NUM_TILES/3);

  //fill red tile objects
  for (uint16_t i = 0; i < (TOTAL_NUM_TILES/3); i++) {
    tile_init_red(&tile[i],i);
  }

  //fill green tile objects
  for (uint16_t i = 0; i < (TOTAL_NUM_TILES/3); i++) {
    tile_init_green(&tile[i+(TOTAL_NUM_TILES/3)],i);
  }

  //fill blue tile objects
  for (uint16_t i = 0; i < (TOTAL_NUM_TILES/3); i++) {
    tile_init_blue(&tile[i+(TOTAL_NUM_TILES - (TOTAL_NUM_TILES/3))],i);
  }
}

// Tick the game control logic
void gameControl_tick() {
    ball_tick();
    paddle_tick();
}
