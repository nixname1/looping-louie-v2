#include <stdlib.h>

#include "stm32f4xx.h"

#include "ll_system.h"
#include "ll_motor.h"
#include "ll_reset_switch.h"


void SysTick_Handler(void);
static volatile uint32_t delay_timer;

/**
 * @brief initializes all modules needed for the game
 */
void ll_system_init()
{
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/SYSTICK_TIMER_SPEED);
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  ll_system_rand_init();
  // init the reset switch
  //ll_reset_switch_init();

  // init the motor system
  ll_motor_init();
}

/**
 * @brief start a new round in the running game
 */
void ll_system_round_start()
{
  // TODO: implement
}

/**
 * @brief stop the round - but keep the game up
 */
void ll_system_round_stop()
{
  // TODO: implement
}

/**
 * @brief start a new game
 */
void ll_system_game_start()
{
  // TODO: implement
}

/**
 * @brief stop the actual game
 */
void ll_system_game_stop()
{
  // TODO: implement
}

/**
 *
 */

void ll_system_rand_init()
{
  uint32_t adc_val;
  uint32_t tmp;
  uint32_t i;

  RCC->APB2ENR |= RCC_APB2ENR_ADC1EN; // clock adc1
  ADC1->CR2 |= ADC_CR2_ADON; // enable adc
  ADC->CCR |= ADC_CCR_TSVREFE; // enable temperature sensor
  ADC1->SQR1 = ADC_SQR1_SQ16; // internal temp. sensor channel
  ADC1->CR2 |= ADC_CR2_SWSTART; // start adc one time
  while(!(ADC1->SR & ADC_SR_EOC)); // wait for adc

  tmp = ADC1->DR;
  for(i = 0; i < tmp/10; i++)
  {
	ADC1->CR2 |= ADC_CR2_SWSTART; // start adc one time
	while(!(ADC1->SR & ADC_SR_EOC)); // wait for adc

	adc_val = ADC1->DR;
	srand(adc_val);
  }

}


/**
 * @brief wait for X cycles of selected systick
 * @param ticks wait for X systicks
 */
void delay(volatile uint32_t ticks)
{
  while(ticks--);
}
/**
 * @brief stop the code execution for x milliseconds
 * @param ms amount of milliseconds to wait
 */
void delay_ms(volatile uint32_t ms)
{
  delay_timer  = (SYSTICK_TIMER_SPEED / 1000) * ms;
  // wait until delay_timer was decreased to 0
  while(delay_timer);
}
/**
 * @brief stop the code execution for x microseconds
 * @param us amount of microseconds to wait
 */
void delay_us(volatile uint32_t us)
{
  delay_timer  = (SYSTICK_TIMER_SPEED / 1000000) * us;
  // wait until delay_timer was decreased to 0
  while(delay_timer);
}

/**
 * @brief the systick handler
 */
void __attribute__ ((section(".after_vectors")))
SysTick_Handler(void)
{
  // only if timer is set - decrement the given value
  if(delay_timer)
  {
	delay_timer--;
  }
}
