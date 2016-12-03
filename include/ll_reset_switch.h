#ifndef _LL_RESET_SWITCH_H_
#define _LL_RESET_SWITCH_H_

#include "stm32f411xe.h"
#include "ll_external.h"

#define LL_RESET_SWITCH_FADE_TIME (5)

void ll_reset_switch_init(void);
void ll_reset_switch_fade_led(void);

void ll_reset_switch_enable(void);
void ll_reset_switch_disable(void);

uint32_t ll_reset_switch_is_enabled(void);
uint32_t ll_reset_switch_is_pushed(void);


void ll_reset_switch_callback(enum ll_ext_event event);

#endif
