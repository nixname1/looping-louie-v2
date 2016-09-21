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
  GPIOA->MODER |= GPIO_MODER_MODER5_0;
  // set to low speed (2MHz)
  GPIOA->OSPEEDR &= ~(GPIO_OSPEEDER_OSPEEDR5);
  GPIOA->AFR[0] |= (GPIO_AF1_TIM2 << (5*4));

  /**
   * initialize the timer
   */
  // enable timer 2
  RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
  // set prescaler value
  // use -1 because the event gets fired one clock later
  TIM2->PSC = 10000 -1; // clock = 100.000.000 Hz -> timer_clk = 10.000 Hz
  // count to 10000
  TIM2->ARR = 10000; // PWM frequency 1 Hz
  TIM2->CCR1 = TIM2->ARR / 2; // duty cycle 0.5 second

  TIM2->CCMR1 |= TIM_CCMR1_OC1M_0 || TIM_CCMR1_OC1M_1;
  TIM2->CCMR1 |= TIM_CCMR1_OC1PE; // enable preload register
  TIM2->CR1 |= TIM_CR1_ARPE; // enable auto-reload preload
  TIM2->EGR |= TIM_EGR_UG; // create update event to load all values to shadow registers

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
