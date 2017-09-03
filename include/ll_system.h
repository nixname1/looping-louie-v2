#ifndef _LL_SYSTEM_H_
#define _LL_SYSTEM_H_

#include "stm32f411xe.h"

#define SYSTICK_SPEED_1ms 	(1000)
#define SYSTICK_SPEED_100us (10000)
#define SYSTICK_SPEED_10us	(100000)
#define SYSTICK_SPEED_1us	(1000000)
#define SYSTICK_TIMER_SPEED SYSTICK_SPEED_100us

enum ll_game_step
{
    LL_STEP_RESET_AND_WAIT_FOR_START,
    LL_STEP_GAME_START,
    LL_STEP_GAME_RUN,
    LL_STEP_GAME_STOP,
    LL_STEP_GAME_ERROR
};

enum ll_round_step
{
    LL_STEP_ROUND_WAIT_FOR_START,
    LL_STEP_ROUND_START,
    LL_STEP_ROUND_RUN,
    LL_STEP_ROUND_END,
    LL_STEP_ROUND_ERROR
};

enum ll_round_state
{
    LL_STATE_ROUND_ERROR = -1,
    LL_STATE_ROUND_WAITING,
    LL_STATE_ROUND_STARTING,
    LL_STATE_ROUND_RUNNING,
    LL_STATE_ROUND_STOPPING,
    LL_STATE_ROUND_STOPPED
};

void ll_system_init(void);

/** round handling */
void ll_system_round_start(void);
void ll_system_round_stop(void);

/** game handling */
void ll_system_game_start(void);
void ll_system_game_stop(void);

/** general functions */
void ll_system_rand_init(void);

void ll_system_debug_init(void);
void ll_system_debug_led_on(void);
void ll_system_debug_led_off(void);

void delay_ms(uint32_t ms);
void delay(uint32_t ticks);

uint64_t ll_system_get_systime(void);

#endif
