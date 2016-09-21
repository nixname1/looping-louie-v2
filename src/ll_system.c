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
 * @brief wait for X cycles of selected systick
 * @param ticks wait for X systicks
 */
void delay(volatile uint32_t ticks)
{
  delay_timer = ticks;
  while(delay_timer);
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
 * @param ms amount of microseconds to wait
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

#if defined(USE_HAL_DRIVER)
  //HAL_IncTick();
#endif
}
