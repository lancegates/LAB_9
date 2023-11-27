#ifndef SWITCHES
#define SWITCHES

#include "leds.h"
#include "xil_io.h"
#include "xparameters.h"
#include <stdint.h>

#define SWITCHES_SW0_MASK 0x1
#define SWITCHES_SW1_MASK 0x2
#define SWITCHES_SW2_MASK 0x4
#define SWITCHES_SW3_MASK 0x8

#define GPIO_DATA 0x00
#define GPIO_TRI 0x04
#define ALLON 0xF

static uint32_t readRegister(uint32_t offset) {
  return Xil_In32(XPAR_SLIDE_SWITCHES_BASEADDR + offset);
}

static void writeRegister(uint32_t offset, uint32_t value) {
  Xil_Out32(XPAR_SLIDE_SWITCHES_BASEADDR + offset, value);
}

// Initializes the SWITCHES driver software and hardware.
void switches_init() { writeRegister(GPIO_TRI, ALLON); }

// Returns the current value of all 4 switches as the lower 4 bits of the
// returned value. bit3 = SW3, bit2 = SW2, bit1 = SW1, bit0 = SW0.
uint8_t switches_read() { return readRegister(GPIO_DATA); }

#endif /* SWITCHES */