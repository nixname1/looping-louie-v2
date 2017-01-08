#ifndef _LL_RESET_SWITCH_H_
#define _LL_RESET_SWITCH_H_

#include "stm32f411xe.h"
#include "ll_external.h"

#define LL_RESET_SWITCH_FADE_TIME       (5)
#define LL_RESET_SWITCH_LONG_EVENT_TIME (3000)

typedef void (*ll_reset_switch_push_event_cb)(void);
typedef void (*ll_reset_switch_long_event_cb)(uint64_t event_length);

void ll_reset_switch_init(void);
void ll_reset_switch_run(void);

void ll_reset_switch_fading_enable(uint32_t fade_speed);
void ll_reset_switch_fading_disable(void);
uint32_t ll_reset_switch_is_fading_enabled(void);

uint32_t ll_reset_switch_was_pressed(void);

void ll_reset_switch_set_long_event_callback(ll_reset_switch_long_event_cb cb);
void ll_reset_switch_set_push_event_callback(ll_reset_switch_push_event_cb cb);

#endif
