#ifndef _LL_MOTOR_H_
#define _LL_MOTOR_H_

#include "stm32f411xe.h"

void ll_motor_init(void);
void ll_motor_deinit(void);
int ll_motor_set_new_random_speed(void);
void ll_motor_stop(void);


#endif
