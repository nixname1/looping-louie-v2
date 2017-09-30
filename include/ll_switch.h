#ifndef LL_SWITCH_H
#define LL_SWITCH_H

#include <stdint.h>

void ll_switch_init(void);
uint32_t ll_switch_is_turned_on(void);
void ll_switch_set_virtual_state(uint32_t state);

#endif //LL_SWITCH_H
