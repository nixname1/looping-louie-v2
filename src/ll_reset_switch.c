#include <stdio.h>

#include "stm32f411xe.h"

#include "ll_system.h"
#include "ll_reset_switch.h"

/**
 * @brief initializes the reset switch system
 */
void ll_reset_switch_init ()
{
  /** initialize the GPIO Port */
  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
  GPIOB->MODER |= GPIO_MODER_MODER6_0;
  GPIOB->OSPEEDR &= ~GPIO_OSPEEDER_OSPEEDR6;

  /** initialize the timer */
  RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;
  TIM4->PSC = 96 - 1;  // 1000 clocks per ms
  TIM4->ARR = 200 - 1; // 200us per signal
  TIM4->CCMR1 = TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1; // pwm mode 1
  TIM4->CCER = TIM_CCER_CC1E; // enable ccr
  //TIM4->CR1 |= TIM_CR1_CEN; // enable timer

  /** initialize the GPIO interrupt */

}

/**
 * @brief enables the reset switch
 * enable interrupt for switch and enable the LED fade
 */
void ll_reset_switch_enable()
{
  // TODO: implement interrput
}

/**
 * @brief disables the reset switch
 * disable interrupt and LED fade
 */
void ll_reset_switch_disable()
{
  // TODO: implement
}

/**
 * @brief callback for interrupt
 * @param press_length time the switch was pressed
 */
void ll_reset_switch_callback(int press_length)
{
  // TODO: implement
}
