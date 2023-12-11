
#include "display.h"
#include "gameControl.h"
#include "interrupts.h"
#include "intervalTimer.h"
#include "touchscreen.h"
#include "buttons.h"
#include <stdio.h>

#define TOUCHSCREEN_TIMER_PERIOD 10.0E-3
#define GAME_TIMER_PERIOD 45.0E-3

volatile bool interrupt_flag;

// Interrupt handler for game - use flag method so that it can be interrupted by
// the touchscreen tick while running.
void game_isr() {
  intervalTimer_ackInterrupt(INTERVAL_TIMER_0);
  interrupt_flag = true;
}

// Interrupt handler for touchscreen - tick directly
void touchscreen_isr() {
  intervalTimer_ackInterrupt(INTERVAL_TIMER_1);
  touchscreen_tick();
}

int main() {

  display_init();
  touchscreen_init(TOUCHSCREEN_TIMER_PERIOD);
  buttons_init();
  //gameControl_init();

  // Initialize timer interrupts
  interrupts_init();
  interrupts_register(INTERVAL_TIMER_0_INTERRUPT_IRQ, game_isr);
  interrupts_register(INTERVAL_TIMER_1_INTERRUPT_IRQ, touchscreen_isr);
  interrupts_irq_enable(INTERVAL_TIMER_0_INTERRUPT_IRQ);
  interrupts_irq_enable(INTERVAL_TIMER_1_INTERRUPT_IRQ);

  intervalTimer_initCountDown(INTERVAL_TIMER_0, GAME_TIMER_PERIOD);
  intervalTimer_initCountDown(INTERVAL_TIMER_1, TOUCHSCREEN_TIMER_PERIOD);
  intervalTimer_enableInterrupt(INTERVAL_TIMER_0);
  intervalTimer_enableInterrupt(INTERVAL_TIMER_1);
  intervalTimer_start(INTERVAL_TIMER_0);
  intervalTimer_start(INTERVAL_TIMER_1);

  // Run tick functions
  while (1) {
    // when a tick happens call gamecontrol tick
    if(interrupt_flag)
      {
        interrupt_flag = false;
        gameControl_tick();
      }
    
  }
}