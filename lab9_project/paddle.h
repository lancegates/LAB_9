#ifndef PADDLE
#define PADDLE

#include <stdbool.h>
#include <stdint.h>

//initialize paddle
void paddle_init();

//tack paddle object
void paddle_tick();

//return the angle the ball should go
void paddle_get_angle();

//return if ball hit then set to false
bool paddle_is_hit(); 

// Get the XY location of the paddle
display_point_t paddle_getXY();

#endif /* PADDLE */