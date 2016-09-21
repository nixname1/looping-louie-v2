#include <stm32f4xx.h>
#include <stm32f4xx_hal.h>
#include <stm32f4xx_hal_gpio.h>
#include <stm32f4xx_hal_rcc.h>

#include <stm32f411xe.h>

#include "ll_reset_switch.h"

/**
 * initializes the reset switch
 */
void ll_reset_switch_init()
{
  // TODO: implement
}

/**
 * enables the switch and turns the led ring on
 */
void ll_reset_switch_enable()
{
  // TODO: implement
}

/**
 * disables the switch and turns the led ring off
 */
void ll_reset_switch_disable()
{
  // TODO: implement
}

/**
 * callback getting called if the switch got pressed
 *
 * @param press_length stores how long the switch was pressed
 */
void ll_reset_switch_callback(int press_length)
{
  // TODO: implement
  UNUSED(press_length);
}
