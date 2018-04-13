#ifndef _LL_MOTOR_H_
#define _LL_MOTOR_H_

#include <stdint.h>

enum ll_motor_direction
{
    LL_MOTOR_REVERSE,
    LL_MOTOR_FORWARD,
    LL_MOTOR_STOP
};

typedef void (*ll_motor_set_speed_cb)(uint32_t speed_percent, enum ll_motor_direction direction);

void ll_motor_init(ll_motor_set_speed_cb set_speed_cb);
void ll_motor_run(uint64_t systime);
void ll_motor_stop(void);

#endif
