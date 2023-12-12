#ifndef GAMECONTROL
#define GAMECONTROL

#include <stdbool.h>
#include "tile.h"
#define TOTAL_NUM_TILES 30

typedef struct {
  tile_t tile[TOTAL_NUM_TILES];
} gameTiles_t;

// Initialize the game control logic
// This function will initialize everything else
void gameControl_init();

// Tick the game control logic
void gameControl_tick();

// edit the score
void scoreTile(tile_t *tile);

#endif /* GAMECONTROL */