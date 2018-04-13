#include <stm32f411xe.h>

#include "hardware/ll_system.h"
#include "hardware/ll_gpio.h"

volatile uint64_t last_readout = 0;
uint32_t last_state = 0;
uint32_t ll_gpio_get_switch_state()
{
	if(ll_system_get_systime() > last_readout + 100)
	{
		last_readout = ll_system_get_systime();
		last_state = (GPIOB->IDR & GPIO_IDR_IDR_2) >> 2;
	}
    return last_state;
}

void ll_gpio_init()
{
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
    GPIOB->MODER |= (GPIO_MODER_MODER2_0);
    GPIOC->PUPDR |= GPIO_PUPDR_PUPDR2_1;
}
