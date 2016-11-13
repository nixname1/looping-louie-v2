#include <stdio.h>

#include "stm32f4xx.h"

#include "ll_system.h"
#include "ll_reset_switch.h"

void EXTI2_IRQHandler(void);

/**
 * @brief initializes the reset switch system
 */
void ll_reset_switch_init()
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
  RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
  SYSCFG->EXTICR[0] = SYSCFG_EXTICR1_EXTI2_PB;
  EXTI->RTSR |= EXTI_RTSR_TR2; // rising trigger
  EXTI->IMR |= EXTI_IMR_MR2; // interrupt mask
}

void EXTI2_IRQHandler()
{
  EXTI->PR |= EXTI_PR_PR2;
  NVIC_DisableIRQ(EXTI2_IRQn);

  //TODO: implement


  NVIC_EnableIRQ(EXTI2_IRQn);
}

/**
 * @brief enables the reset switch
 * enable interrupt for switch and enable the LED fade
 */
__inline void ll_reset_switch_enable()
{
  NVIC_EnableIRQ(EXTI2_IRQn);
  // TODO: implement interrput
}

/**
 * @brief disables the reset switch
 * disable interrupt and LED fade
 */
__inline void ll_reset_switch_disable()
{
  NVIC_DisableIRQ(EXTI2_IRQn);
  // TODO: implement
}

/**
 * @brief callback for interrupt
 * @param press_length time the switch was pressed
 */
void ll_reset_switch_callback(int press_length)
{
  UNUSED(press_length);
  // TODO: implement
}
