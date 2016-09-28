#include <stdio.h>
#include <stdlib.h>

#include "stm32f4xx.h"

#include "diag/Trace.h"

#include "ll_system.h"
#include "ll_motor.h"

void TIM2_IRQHandler(void);

int
main (int argc, char* argv[])
{
  UNUSED(argc);
  UNUSED(argv);
  trace_printf("System clock: %u Hz\n", SystemCoreClock);
  trace_puts("**** Starting Looping Louie V2 ****");

  ll_system_init();
  ll_system_game_start();

  // enable clock on GPIO A
  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
  // set as output
  GPIOA->MODER |= GPIO_MODER_MODER5_1;
  // set to low speed (2MHz)
  GPIOA->OSPEEDR &= ~(GPIO_OSPEEDER_OSPEEDR5);
  GPIOA->AFR[0] |= (GPIO_AF1_TIM2 << (5*4));

  /**
   * initialize the timer
   */
  // Wie schon zuvor
  // enable clock to timer 2
  RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;

  // set prescaler value
  TIM2->PSC = 10000 -1; // clock = 100.000.000 Hz -> CK_PSK = 10.000 Hz

  // count to 5000
  TIM2->ARR = 5000 -1; // 10.000 Hz / 5000 = 2 Hz

  TIM2->CCR1 = 2500 -1;


  // Ab hier dann capture compare setup

  // set capture compare mode to "pwm mode 1", i.e. output active if CNT<CCR
  TIM2->CCMR1 = TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1;

  // enable the capture compare output
  TIM2->CCER = TIM_CCER_CC1E;

  // enable counter
  TIM2->CR1 |= TIM_CR1_CEN;


  // Ab hier dann capture compare setup

  // set capture compare mode to "pwm mode 1", i.e. output active if CNT<CCR
  TIM2->CCMR1 = TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1;

  // enable the capture compare output
  TIM2->CCER = TIM_CCER_CC1E;

  // enable counter
  TIM2->CR1 |= TIM_CR1_CEN;

  while (1);

  trace_puts("****** FINISHED ******");
}

void
TIM2_IRQHandler(void)
{
//  // clear interrupt flag
//  TIM2->SR &= ~(TIM_SR_UIF);
//  // toggle Pin
//  GPIOA->ODR ^= GPIO_ODR_ODR_5;
}
