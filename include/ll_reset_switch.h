#ifndef _LL_RESET_SWITCH_H_
#define _LL_RESET_SWITCH_H_

#include "stm32f411xe.h"

void ll_reset_switch_init(void);
void ll_reset_switch_enable(void);
void ll_reset_switch_disable(void);
void ll_reset_switch_callback(int press_length);

#endif
