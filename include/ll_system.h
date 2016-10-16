#ifndef _LL_SYSTEM_H_
#define _LL_SYSTEM_H_

#include "stm32f411xe.h"

/**
 * one clock is 10,4166 ns
 */
#define SYSTICK_SPEED_1ms 	(960)      // every 100.000th clock
#define SYSTICK_SPEED_100us (9600)     // every 10.000th clock
#define SYSTICK_SPEED_10us	(96000)    // every 1000th clock
#define SYSTICK_SPEED_1us	(960000)   // every 100th clock
#define SYSTICK_SPEED_100ns	(9600000)  // every 10th clock
#define SYSTICK_SPEED_41ns  (24000000) // every 4nd clock
#define SYSTICK_SPEED_10ns	(96000000) // every clock
#define SYSTICK_TIMER_SPEED SYSTICK_SPEED_41ns

void ll_system_init(void);

/** round handling */
void ll_system_round_start(void);
void ll_system_round_stop(void);

/** game handling */
void ll_system_game_start(void);
void ll_system_game_stop(void);

/** general functions */
void ll_system_rand_init(void);

void delay_ms(uint32_t ms);
void delay_us(uint32_t us);
void delay(uint32_t ticks);

#endif
