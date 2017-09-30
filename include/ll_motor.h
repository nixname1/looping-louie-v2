#ifndef _LL_MOTOR_H_
#define _LL_MOTOR_H_

#include <stdint.h>

enum ll_motor_direction
{
    LL_MOTOR_REVERSE,
    LL_MOTOR_FORWARD,
    LL_MOTOR_STOP
};

void ll_motor_init(void);
void ll_motor_deinit(void);
int ll_motor_set_new_random_speed(void);
int ll_motor_set_speed(uint32_t val, uint32_t dir);
void ll_motor_run(void);
void ll_motor_stop(void);

#endif
