#ifndef LL_PWM_H
#define LL_PWM_H

void ll_pwm_init(void);
void ll_pwm_set_speed_in_percent(uint32_t speed_percent, enum ll_motor_direction dir);

#endif
