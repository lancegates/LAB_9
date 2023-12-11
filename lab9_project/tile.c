#include "tile.h"
#include "ball.h"
#include "display.h"
#include "paddle.h"

// number of tiles per row
uint16_t numTiles;

// There are 3 pixel gaps between the tile sets
#define RED_Y 0
#define GREEN_Y 18
#define BLUE_Y 36

// determine tile length based on the number of tiles
#define TILE_LENGTH 320 / numTiles
// all tiles are 15 pixels tall
#define TILE_WIDTH 15
// gap between tile side edges
#define TILE_GAP 3

// function to edit num tiles from TOTAL_NUM_TILES in gameControl
void editNumTiles(uint16_t num) { numTiles = num; }

// Draws tiles given 0 (red), 1 (green), 2 (blue)
void drawTile(uint8_t tileType, uint16_t tilePosition) {
  // If drawing red (0)...
  if (tileType == 0) {
    display_fillRect(((TILE_LENGTH / 2) + (tilePosition * TILE_LENGTH)) -
                         (TILE_LENGTH / 2),
                     RED_Y, TILE_LENGTH - TILE_GAP, TILE_WIDTH, DISPLAY_RED);
  }
  // If drawing green (1)...
  else if (tileType == 1) {
    display_fillRect(
        ((TILE_LENGTH / 2) + (tilePosition * TILE_LENGTH)) - (TILE_LENGTH / 2),
        GREEN_Y, TILE_LENGTH - TILE_GAP, TILE_WIDTH, DISPLAY_GREEN);
  }
  // If drawing blue (2)...
  else if (tileType == 2) {
    display_fillRect(((TILE_LENGTH / 2) + (tilePosition * TILE_LENGTH)) -
                         (TILE_LENGTH / 2),
                     BLUE_Y, TILE_LENGTH - TILE_GAP, TILE_WIDTH, DISPLAY_BLUE);
  }
}

void tile_init_red(tile_t *tile, uint16_t x) {
  // set tile type
  tile->type = TILE_TYPE_RED;

  // set x,y origin starting spot
  tile->x_position = x*(TILE_LENGTH + TILE_GAP);
  tile->y_position = RED_Y;

  // draw the tile
  drawTile(0, x);
}

void tile_init_green(tile_t *tile, uint16_t x) {
  // set tile type
  tile->type = TILE_TYPE_GREEN;

  // set x,y origin starting spot
  tile->x_position = x*(TILE_LENGTH + TILE_GAP);
  tile->y_position = GREEN_Y;

  // draw the tile
  drawTile(1, x);
}

void tile_init_blue(tile_t *tile, uint16_t x) {
  // set tile type
  tile->type = TILE_TYPE_BLUE;

  // set x,y origin starting spot
  tile->x_position = x*(TILE_LENGTH + TILE_GAP);
  tile->y_position = BLUE_Y;

  // draw the tile
  drawTile(2, x);
}

bool tile_is_dead(tile_t *tile) { return tile->is_dead; }

// draw tile (3)
void eraseTile(tile_t *tile) {
  display_fillRect(tile->x_position,tile->y_position, TILE_LENGTH - TILE_GAP, TILE_WIDTH, DISPLAY_BLACK);
}