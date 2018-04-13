
#include <stm32f4xx.h>

#include "ll_motor.h"
#include "hardware/ll_system.h"
#include "hardware/ll_pwm.h"

void ll_pwm_init()
{
	/** initialize the GPIO Port */
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
	GPIOC->MODER |= GPIO_MODER_MODER6_1;
	GPIOC->OSPEEDR &= ~GPIO_OSPEEDER_OSPEEDR6;
	GPIOC->AFR[0] |= (GPIO_AF2_TIM3 << (6 * 4));

	/** initialize the timer */
	RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
	TIM3->PSC = 96 - 1;	// 1000 clocks per ms
	TIM3->ARR = 20000 - 1; // 20ms per signal
	TIM3->CCMR1 = TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1; // pwm mode 1
	TIM3->CCER = TIM_CCER_CC1E; // enable ccr
	TIM3->CR1 |= TIM_CR1_CEN; // enable timer
}

/**
 * @param val	expects the speed of the motor in percent
 * @param dir	the direction of the motor
 */
void ll_pwm_set_speed_in_percent(uint32_t speed_percent, enum ll_motor_direction dir)
{
	static enum ll_motor_direction old_direction = LL_MOTOR_STOP;
	uint32_t val = 0;

	if(dir == LL_MOTOR_FORWARD)
	{
		speed_percent = 100 - speed_percent;
		val = 1180 + (uint32_t) ((double) speed_percent * 2.1);
	}
	else if(dir == LL_MOTOR_STOP)
	{
		val = 1430;
	}
	else
	{
		if(old_direction != LL_MOTOR_REVERSE)
		{
			TIM3->CCR1 = 1850;
			delay_ms(100);
			TIM3->CCR1 = 1430;
			delay_ms(100);
		}
		val = 1590 + (uint32_t) ((double) speed_percent * 4.6);
	}
	TIM3->CCR1 = val;
	old_direction = dir;
}
