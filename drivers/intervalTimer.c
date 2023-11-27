#ifndef INTERVALTIMER
#define INTERVALTIMER

#include "intervalTimer.h"
#include "xil_io.h"
#include "xparameters.h"
#include <stdbool.h>
#include <stdint.h>

#define INTERVAL_TIMER_0 0
#define INTERVAL_TIMER_1 1
#define INTERVAL_TIMER_2 2

#define STOP 0

#define INTERVAL_TIMER_0_INTERRUPT_IRQ 0
#define INTERVAL_TIMER_1_INTERRUPT_IRQ 1
#define INTERVAL_TIMER_2_INTERRUPT_IRQ 2

#define TCSR0 0x00
#define TLR0 0x04
#define TCR0 0x08

#define TCSR1 0x10
#define TLR1 0x14
#define TCR1 0x18

#define CASC 0x800
#define ENT 0x80
#define LOAD 0x20
#define CAPT 0x8
#define UDT 0x2
#define ARHT 0x10
#define ENIT 0x40
#define T0INT 0x100

#define PERIODCONST 100000000
#define SHIFT32 32

static uint32_t readRegister(uint8_t timerNumber, uint32_t offset) {
  // timer 0,1,2 read
  if (timerNumber == INTERVAL_TIMER_0) {
    return Xil_In32(XPAR_AXI_TIMER_0_BASEADDR + offset);
  } else if (timerNumber == INTERVAL_TIMER_1) {
    return Xil_In32(XPAR_AXI_TIMER_1_BASEADDR + offset);
  } else if (timerNumber == INTERVAL_TIMER_2) {
    return Xil_In32(XPAR_AXI_TIMER_2_BASEADDR + offset);
  }
}
static void writeRegister(uint8_t timerNumber, uint32_t offset,
                          uint32_t value) {
  // timer 0,1,2 write
  if (timerNumber == INTERVAL_TIMER_0) {
    return Xil_Out32(XPAR_AXI_TIMER_0_BASEADDR + offset, value);
  } else if (timerNumber == INTERVAL_TIMER_1) {
    return Xil_Out32(XPAR_AXI_TIMER_1_BASEADDR + offset, value);
  } else if (timerNumber == INTERVAL_TIMER_2) {
    return Xil_Out32(XPAR_AXI_TIMER_2_BASEADDR + offset, value);
  }
}

// This function is called whenever you want to reload the Counter values
// from the load registers.  For a count-up timer, this will reset the
// timer to zero.  For a count-down timer, this will reset the timer to
// its initial count-down value.  The load registers should have already
// been set in the appropriate `init` function, so there is no need to set
// them here.  You just need to enable the load (and remember to disable it
// immediately after otherwise you will be loading indefinitely).
void intervalTimer_reload(uint32_t timerNumber) {
  // set LOAD0 of TCSR0 to 1
  writeRegister(timerNumber, TCSR0, readRegister(timerNumber, LOAD) | LOAD);

  // disable load immediately after
  writeRegister(timerNumber, TCSR0, readRegister(timerNumber, LOAD) & ~(LOAD));
}

// You must configure the interval timer before you use it:
// 1. Set the Timer Control/Status Registers such that:
//  - The timer is in 64-bit cascade mode
//  - The timer counts up
// 2. Initialize both LOAD registers with zeros
// 3. Call the _reload function to move the LOAD values into the Counters
void intervalTimer_initCountUp(uint32_t timerNumber) {
  // initialize TCSR0 register
  writeRegister(timerNumber, TCSR0, 0);

  // initialize TCSR1 register
  writeRegister(timerNumber, TCSR1, 0);

  // enable cascade CASC -> 1 in the TCSR0 register
  writeRegister(timerNumber, TCSR0, readRegister(timerNumber, TCSR0) | CASC);

  // enable count up UDT0 -> 0 in the TCSR0 register
  writeRegister(timerNumber, TCSR0, readRegister(timerNumber, TCSR0) & ~(UDT));

  // initialize load with 0
  writeRegister(timerNumber, TLR0, 0);
  writeRegister(timerNumber, TLR1, 0);

  // call reload
  intervalTimer_reload(timerNumber);
}

// You must configure the interval timer before you use it:
// 1. Set the Timer Control/Status Registers such that:
//  - The timer is in 64-bit cascade mode
//  - The timer counts down
//  - The timer automatically reloads when reaching zero
// 2. Initialize LOAD registers with appropriate values, given the `period`.
// 3. Call the _reload function to move the LOAD values into the Counters
void intervalTimer_initCountDown(uint32_t timerNumber, double period) {
  // initialize TCSR0 register
  uint64_t t = PERIODCONST * period;
  writeRegister(timerNumber, TCSR0, 0);

  // initialize TCSR1 register
  writeRegister(timerNumber, TCSR1, 0);

  // enable cascade CASC -> 1 in the TCSR0 register
  writeRegister(timerNumber, TCSR0, readRegister(timerNumber, TCSR0) | CASC);

  // enable count down UDT0 -> 1 in the TCSR0 register
  writeRegister(timerNumber, TCSR0, readRegister(timerNumber, TCSR0) | UDT);

  // enable automatic reload
  writeRegister(timerNumber, TCSR0, readRegister(timerNumber, TCSR0) | ARHT);

  // write LOAD registers given period
  writeRegister(timerNumber, TLR0, readRegister(timerNumber, TLR0) | t);
  writeRegister(timerNumber, TLR1,
                readRegister(timerNumber, TLR1) | (t >> SHIFT32));

  // reload registers
  intervalTimer_reload(timerNumber);
}

// This function starts the interval timer running.
// If the interval timer is already running, this function does nothing.
// timerNumber indicates which timer should start running.
// Make sure to only change the Enable Timer bit of the register and not modify
// the other bits.
void intervalTimer_start(uint32_t timerNumber) {
  // make ENT0 a 1
  writeRegister(timerNumber, TCSR0, readRegister(timerNumber, TCSR0) | ENT);
}

// This function stops a running interval timer.
// If the interval time is currently stopped, this function does nothing.
// timerNumber indicates which timer should stop running.
// Make sure to only change the Enable Timer bit of the register and not modify
// the other bits.
void intervalTimer_stop(uint32_t timerNumber) {
  // make ENT0 a 0
  writeRegister(timerNumber, TCSR0, readRegister(timerNumber, TCSR0) & ~(ENT));
}

// Use this function to ascertain how long a given timer has been running.
// Note that it should not be an error to call this function on a running timer
// though it usually makes more sense to call this after intervalTimer_stop()
// has been called. The timerNumber argument determines which timer is read.
// In cascade mode you will need to read the upper and lower 32-bit registers,
// concatenate them into a 64-bit counter value, and then perform the conversion
// to a double seconds value.
double intervalTimer_getTotalDurationInSeconds(uint32_t timerNumber) {
  uint64_t count = 0x0000000000000000;
  if (timerNumber == INTERVAL_TIMER_0) {
    // read TCR1
    count = count | readRegister(timerNumber, TCR1);
    count = count << SHIFT32;
    // read TCR0
    count = count | readRegister(timerNumber, TCR0);
    double freq = XPAR_AXI_TIMER_0_CLOCK_FREQ_HZ;
    return count / freq;
  } else if (timerNumber == INTERVAL_TIMER_1) {
    // read TCR1
    count = count | readRegister(timerNumber, TCR1);
    count = count << SHIFT32;
    // read TCR0
    count = count | readRegister(timerNumber, TCR0);
    double freq = XPAR_AXI_TIMER_1_CLOCK_FREQ_HZ;
    return count / freq;
  } else if (timerNumber == INTERVAL_TIMER_2) {
    // read TCR1
    count = count | readRegister(timerNumber, TCR1);
    count = count << SHIFT32;
    // read TCR0
    count = count | readRegister(timerNumber, TCR0);
    double freq = XPAR_AXI_TIMER_2_CLOCK_FREQ_HZ;
    return count / freq;
  }
}

// Enable the interrupt output of the given timer.
void intervalTimer_enableInterrupt(uint8_t timerNumber) {
  writeRegister(timerNumber, TCSR0, readRegister(timerNumber, TCSR0) | ENIT);
}

// Disable the interrupt output of the given timer.
void intervalTimer_disableInterrupt(uint8_t timerNumber) {
  writeRegister(timerNumber, TCSR0, readRegister(timerNumber, TCSR0) & ~(ENIT));
}

// Acknowledge the rollover to clear the interrupt output.
void intervalTimer_ackInterrupt(uint8_t timerNumber) {
  writeRegister(timerNumber, TCSR0, readRegister(timerNumber, TCSR0) | T0INT);
}

#endif