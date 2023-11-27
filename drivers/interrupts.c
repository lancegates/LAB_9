/*
This software is provided for student assignment use in the Department of
Electrical and Computer Engineering, Brigham Young University, Utah, USA.

Users agree to not re-host, or redistribute the software, in source or binary
form, to other persons or other institutions. Users may modify and use the
source code for personal or educational use.

For questions, contact Brad Hutchings or Jeff Goeders, https://ece.byu.edu/
*/

#include "interrupts.h"
#include "armInterrupts.h"
#include "intervalTimer.h"
#include "xil_io.h"
#include <leds.h>
#include <stdint.h>
#include <xparameters.h>

#define IPR 0x04 // interrupt pending register
#define IER 0x08 // interrupt enable register
#define IAR 0x0C // interrupt acknowledge register
#define SIE 0x10 // set interrupt enables
#define CIE 0x14 // clear interrupt enables
#define MER 0x1C // master enable register

#define MER_ENABLE 0x03 // MER enable mask
#define PTRPLACES 3

// initialize pointer addresses
static void (*isrFcnPtrs[PTRPLACES])() = {NULL};

// helper funtion for reading a register
static uint32_t readRegister(uint32_t offset) {
  return Xil_In32(XPAR_AXI_INTC_0_BASEADDR + offset);
}

// helper function for writing to a register
static void writeRegister(uint32_t offset, uint32_t value) {
  Xil_Out32(XPAR_AXI_INTC_0_BASEADDR + offset, value);
}

// identify address of interrupt using this function
static void interrupts_isr() {
  for (uint8_t i = 0; i < PTRPLACES; ++i) {
    // Check if it has an interrupt pending
    if (readRegister(IPR) & (1 << i)) {
      // Check if there is a callback
      if (isrFcnPtrs[i] != NULL) {
        // Call the callback function
        isrFcnPtrs[i]();
      }
      // Acknowledge interrupt
      writeRegister(IAR, 1 << i);
    }
  }
}

// Initialize interrupt hardware
// This function should:
// 1. Configure AXI INTC registers to:
//  - Enable interrupt output (see Master Enable Register)
//  - Disable all interrupt input lines.
// 2. Enable the Interrupt system on the ARM processor, and register an ISR
// handler function. This is done by calling:
//  - armInterrupts_init()
//  - armInterrupts_setupIntc(isr_fcn_ptr)
//  - armInterrupts_enable()
void interrupts_init() {
  // enable interrupt output
  writeRegister(MER, readRegister(MER) | MER_ENABLE);
  // disable all interrupt input lines
  writeRegister(IER, 0);
  // enable interrupt on ARM
  armInterrupts_init();
  // setup Intc
  armInterrupts_setupIntc(interrupts_isr);
  // enable arm interrupts
  armInterrupts_enable();
}

// Register a callback function (fcn is a function pointer to this callback
// function) for a given interrupt input number (irq).  When this interrupt
// input is active, fcn will be called.
void interrupts_register(uint8_t irq, void (*fcn)()) { isrFcnPtrs[irq] = fcn; }

// Enable single input interrupt line, given by irq number.
void interrupts_irq_enable(uint8_t irq) { writeRegister(SIE, 1 << irq); }

// Disable single input interrupt line, given by irq number.
void interrupts_irq_disable(uint8_t irq) { writeRegister(CIE, 1 << irq); }
