#include "stm32f4xx.h"

#include "ll_system.h"
#include "ll_motor.h"

/**
 * @brief initializes the motor module
 * sets up a timer on the specified LL_MOTOR_PIN on LL_MOTOR_PORT
 */
void ll_motor_init()
{
  /** initialize the GPIO Port */
  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
  GPIOC->MODER |= GPIO_MODER_MODER6_1 | GPIO_MODER_MODER8_0;
  GPIOC->OSPEEDR &= ~(GPIO_OSPEEDER_OSPEEDR6 | GPIO_OSPEEDER_OSPEEDR8);
  GPIOC->AFR[0] |= (GPIO_AF2_TIM3 << (6*4));

  /**
   * initialize the timer
   */
  RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
  TIM3->PSC = 96 -1;
  TIM3->ARR = 20000 -1; // 1000 clocks per ms
  TIM3->CCMR1 = TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1; // pwm mode 1
  TIM3->CCER = TIM_CCER_CC1E; // enable ccr
  TIM3->CR1 |= TIM_CR1_OPM & TIM_CR1_CEN; // enable timer
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
  uint32_t i;
  uint32_t val;
  uint32_t direction;


  val = rand();

  if(direction)
  {
	/* revert mode */
	// toggle motor esc pin
	GPIOC->BSRR = GPIO_BSRR_BR_8;
	delay_ms(10);
	GPIOC->BSRR = GPIO_BSRR_BS_8;
  }

  TIM3->CCR1 = val;
  trace_printf("setting speed: %ul %s\n", val, direction ? "backward" : "forward");

  return 0;
}

/**
 * @brief stops the motor
 */
void ll_motor_stop()
{
  ll_motor_set_speed(0, 0);
}
