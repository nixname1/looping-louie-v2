#ifndef _LL_SYSTEM_H_
#define _LL_SYSTEM_H_

#define SYSTICK_SPEED_1ms 	(1000)
#define SYSTICK_SPEED_100us (10000)
#define SYSTICK_SPEED_10us	(100000)
#define SYSTICK_SPEED_1us	(1000000)
#define SYSTICK_TIMER_SPEED SYSTICK_SPEED_100us

void ll_system_init(void);

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
