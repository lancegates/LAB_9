#ifndef GAMECONTROL
#define GAMECONTROL

#include <stdbool.h>

#define TOTAL_NUM_TILES 30

tile_t tile[TOTAL_NUM_TILES];

// Initialize the game control logic
// This function will initialize everything else
void gameControl_init();

// Tick the game control logic
void gameControl_tick();

#endif /* GAMECONTROL */