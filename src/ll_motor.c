#include <stdlib.h>

#include "stm32f4xx.h"
#include "diag/Trace.h"

#include "ll_system.h"
#include "ll_motor.h"

static uint32_t mg_next_speed_change = 0;

/**
 * @brief initializes the motor module
 * sets up a timer with output to a pin
 */
void ll_motor_init()
{
  /** initialize the GPIO Port */
  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
  GPIOC->MODER |= GPIO_MODER_MODER6_1;
  GPIOC->OSPEEDR &= ~GPIO_OSPEEDER_OSPEEDR6;
  GPIOC->AFR[0] |= (GPIO_AF2_TIM3 << (6*4));

  /**
   * initialize the timer
   */
  RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
  TIM3->PSC = 96 -1;	// 1000 clocks per ms
  TIM3->ARR = 20000 -1; // 20ms per signal
  TIM3->CCMR1 = TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1; // pwm mode 1
  TIM3->CCER = TIM_CCER_CC1E; // enable ccr
  TIM3->CR1 |= TIM_CR1_CEN; // enable timer
}

/**
 * @brief
 * @param val	expects the speed of the motor in precent
 * @param dir	the direction of the motor
 * 				0=forward 1=backward
 * @retval		0 on success, -1 on failure
 */
int ll_motor_set_speed(uint32_t val, uint32_t dir)
{
  UNUSED(val);
  UNUSED(dir);

  return 0;
}

/**
 * @brief
 * @param val	expects the speed of the motor in precent
 * @param dir	the direction of the motor
 * 				0=forward 1=backward
 * @retval		0 on success, -1 on failure
 */
int ll_motor_set_new_random_speed()
{
  enum ll_motor_direction direction;
  uint32_t val;
  int rand_tmp;

  // generate a direction
  rand_tmp = rand();
  if(rand_tmp <= ((RAND_MAX / 100) * 1)) // a change of 10% to get reverse
  {
	  direction = LL_MOTOR_REVERSE;
  }

  rand_tmp = rand() % 100;
  // generate a speed
  switch(direction)
  {
  case LL_MOTOR_REVERSE:
	  // use motor brake (~50% backward for ~10 ms)
	  TIM3->CCR1 = 1850;
	  delay_ms(100);
	  // send zero
	  TIM3->CCR1 = 1410;
	  delay_ms(100);
	  // and then we are able to send the speed backward
	  val = 1680 + (uint32_t)((double) rand_tmp * 3.8);

	  trace_printf("setting speed: %u reverse\n", val);
	  break;

  case LL_MOTOR_FORWARD:

	  break;

  case LL_MOTOR_STOP:
	  // in case of stop - set to zero
	  val = 1410;
	  break;
  }

  TIM3->CCR1 = val;

  return 0;
}

/**
 * @brief runs the motor
 */
void ll_motor_run()
{
  if(mg_next_speed_change <= ll_system_get_ticks() )
  {
	ll_motor_set_new_random_speed();
  }
}

/**
 * @brief stops the motor
 */
void ll_motor_stop()
{
  ll_motor_set_speed(0, 0);
}
