#ifndef BUTTONS
#define BUTTONS

#include "leds.h"
#include "xil_io.h"
#include "xparameters.h"
#include <stdint.h>

#define BUTTONS_BTN0_MASK 0x1
#define BUTTONS_BTN1_MASK 0x2
#define BUTTONS_BTN2_MASK 0x4
#define BUTTONS_BTN3_MASK 0x8

#define GPIO_DATA 0x00
#define GPIO_TRI 0x04
#define ALLON 0xF

static uint32_t readRegister(uint32_t offset) {
  return Xil_In32(XPAR_PUSH_BUTTONS_BASEADDR + offset);
}

static void writeRegister(uint32_t offset, uint32_t value) {
  Xil_Out32(XPAR_PUSH_BUTTONS_BASEADDR + offset, value);
}

// Initializes the button driver software and hardware.
void buttons_init() { writeRegister(GPIO_TRI, ALLON); }

// Returns the current value of all 4 buttons as the lower 4 bits of the
// returned value. bit3 = BTN3, bit2 = BTN2, bit1 = BTN1, bit0 = BTN0.
uint8_t buttons_read() { return readRegister(GPIO_DATA); }

#endif /* BUTTONS */
