#include <stm32f411xe.h>

#include "hardware/ll_gpio.h"

uint32_t ll_gpio_get_switch_state()
{
    uint32_t state = (GPIOB->IDR & GPIO_IDR_IDR_2) >> 2;
    return state;
}

void ll_gpio_init()
{
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
    GPIOB->MODER |= (GPIO_MODER_MODER2_0);
    GPIOC->PUPDR |= GPIO_PUPDR_PUPDR2_1;
}
