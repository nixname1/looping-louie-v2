#include <stm32f411xe.h>

#include "ll_switch.h"

void ll_switch_init()
{
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
    GPIOB->MODER |= (GPIO_MODER_MODER2_0);
    GPIOC->PUPDR |= GPIO_PUPDR_PUPDR2_1;
}

uint32_t ll_switch_is_turned_on()
{
    uint32_t state = (GPIOB->IDR & GPIO_IDR_IDR_2) >> 2;
    return state;
}