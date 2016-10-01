#ifndef _LL_SYSTEM_H_
#define _LL_SYSTEM_H_

#include "stm32f411xe.h"

#define SYSTICK_SPEED_1ms 	(1000)      // every 100.000th clock
#define SYSTICK_SPEED_100us (10000)     // every 10.000th clock
#define SYSTICK_SPEED_10us	(100000)    // every 1000th clock
#define SYSTICK_SPEED_1us	(1000000)   // every 100th clock
#define SYSTICK_SPEED_100ns	(10000000)  // every 10th clock
#define SYSTICK_SPEED_10ns	(100000000) // every clock
#define SYSTICK_TIMER_SPEED SYSTICK_SPEED_1us

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
