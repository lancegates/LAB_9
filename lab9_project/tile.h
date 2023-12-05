#ifndef TILE
#define TILE

#include <stdbool.h>
#include <stdint.h>

typedef enum {
  TILE_TYPE_RED,
  TILE_TYPE_GREEN,
  TILE_TYPE_BLUE
} tile_type_t;

typedef struct {

  tile_type_t type;

  uint16_t x_position;

  uint16_t y_position;

  bool destroy_me;

} tile_t;

void tile_init_red(tile_t *tile);


void tile_init_green(tile_t *tile);


void tile_init_blue(tile_t *tile);


bool tile_is_dead(tile_t *tile);


#endif /* TILE */