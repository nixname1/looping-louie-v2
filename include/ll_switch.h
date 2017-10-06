#ifndef LL_SWITCH_H
#define LL_SWITCH_H

#include <stdint.h>

typedef uint32_t(*ll_switch_get_state_cb)(void);

void ll_switch_init(ll_switch_get_state_cb read_state_cb);
uint32_t ll_switch_is_turned_on(void);

#endif
