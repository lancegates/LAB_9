#include "paddle.h"
#include "ball.h"
#include "tile.h"
#include "display.h"

//There are 3 pixel gaps between the tiles
#define RED_Y 0
#define GREEN_Y 18
#define BLUE_Y 36

void tile_init_red(tile_t *tile, uint16_t x) {
  // set tile type
  tile->type = TILE_TYPE_RED;

  // set x,y origin starting spot
  tile->x_position = x;
  tile->y_position = RED_Y;
}

void tile_init_green(tile_t *tile);


void tile_init_blue(tile_t *tile);


bool tile_is_dead(tile_t *tile);