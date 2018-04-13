#include <stdlib.h>
#include <stdint.h>
#include "ll_switch.h"

ll_switch_get_state_cb get_state_cb = NULL;

void ll_switch_init(ll_switch_get_state_cb read_state_cb)
{
	get_state_cb = read_state_cb;
}

uint32_t ll_switch_is_turned_on()
{
	return get_state_cb();
}