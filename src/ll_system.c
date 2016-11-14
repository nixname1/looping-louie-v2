#include <stdlib.h>

#include "stm32f4xx.h"

#include "ll_motor.h"
#include "ll_reset_switch.h"
#include "ll_system.h"

void SysTick_Handler(void);
static volatile uint32_t delay_timer;
static volatile uint64_t systick_counter; // stores up to 59973028 years
/**
 * @brief initializes all modules needed for the game
 */
void ll_system_init()
{
    NVIC_SetPriority(SysTick_IRQn, 6);
    HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq() / SYSTICK_TIMER_SPEED);
    HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

    // set the internal system counter to zero
    systick_counter = 0;
#ifdef DEBUG
    ll_system_debug_init();
#endif

    ll_system_rand_init();
    ll_motor_init();
    // ll_reset_switch_init();
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
 * @brief initialize rand()
 */
void ll_system_rand_init()
{
    uint32_t adc_val;
    uint32_t tmp;
    uint32_t i;

    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN; // clock adc1
    ADC1->CR2 |= ADC_CR2_ADON;          // enable adc
    ADC->CCR |= ADC_CCR_TSVREFE;        // enable temperature sensor
    ADC1->SQR1 = ADC_SQR1_SQ16;         // internal temp. sensor channel
    ADC1->CR2 |= ADC_CR2_SWSTART;       // start adc one time
    while (!(ADC1->SR & ADC_SR_EOC))
        ; // wait for adc

    tmp = ADC1->DR;
    for (i = 0; i < tmp / 10; i++)
    {
        ADC1->CR2 |= ADC_CR2_SWSTART; // start adc one time
        while (!(ADC1->SR & ADC_SR_EOC))
            ; // wait for adc
    }

    adc_val = ADC1->DR;
    srand(adc_val);

    // disable ADC again - not needed anymore
    RCC->APB2ENR &= ~RCC_APB2ENR_ADC1EN; // clock adc1
}

/**
 * initializes the onboard LED for debugging
 */
void ll_system_debug_init()
{
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    GPIOA->MODER |= GPIO_MODER_MODER5_0;
    GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR5; // highest speed
}
/**
 * set debug LED on
 */
__inline void ll_system_debug_led_on()
{
    GPIOA->BSRR = GPIO_BSRR_BS_5;
}
/**
 * set debug LED off
 */
__inline void ll_system_debug_led_off()
{
    GPIOA->BSRR = GPIO_BSRR_BR_5;
}

/**
 * @brief wait for X cycles of selected systick
 * @param ticks wait for X systicks
 */
void delay(volatile uint32_t ticks)
{
    while (ticks--)
        ;
}
/**
 * @brief stop the code execution for x milliseconds
 * @param ms amount of milliseconds to wait
 */
void delay_ms(volatile uint32_t ms)
{
    delay_timer = (SYSTICK_TIMER_SPEED / 100) * ms;
    delay_timer += 5;
    delay_timer /= 10;

    // wait until delay_timer was decreased to 0
    while (delay_timer)
        ;
}

__inline uint64_t ll_system_get_ticks()
{
    return systick_counter;
}
/**
 * @brief the systick handler
 */
void __attribute__((section(".after_vectors"))) SysTick_Handler(void)
{
    // increment systic counter
    systick_counter++;
    // only if timer is set - decrement the given value
    if (delay_timer)
    {
        delay_timer--;
    }
}
