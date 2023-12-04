#ifndef PADDLE
#define PADDLE

#include <stdbool.h>
#include <stdint.h>

/* This struct contains all information about th paddle*/
typedef struct {

    // paddle center
    int16_t x_current;
    int16_t y_current;

 

} paddle_t;

#endif /* PADDLE */