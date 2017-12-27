#include <stdlib.h>

#include "stm32f4xx.h"

#include "hardware/ll_system.h"

void SysTick_Handler(void);
static volatile uint32_t delay_timer;
static volatile uint64_t systick_counter;
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
}

/**
 * @brief initialize random()
 */
void ll_system_random_init()
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
    srandom(adc_val);

    // disable ADC again - not needed anymore
    RCC->APB2ENR &= ~RCC_APB2ENR_ADC1EN; // clock adc1
}

/**
 * initializes the onboard LED for debugging
 */
void ll_system_debug_init()
{
#ifdef DEBUG
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    GPIOA->MODER |= GPIO_MODER_MODER5_0;
    GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR5; // highest speed
#endif
}
/**
 * set debug LED on
 */
__inline void ll_system_debug_led_on()
{
#ifdef DEBUG
    GPIOA->BSRR = GPIO_BSRR_BS_5;
#endif
}
/**
 * set debug LED off
 */
__inline void ll_system_debug_led_off()
{
#ifdef DEBUG
    GPIOA->BSRR = GPIO_BSRR_BR_5;
#endif
}

/**
 * @brief wait for X cycles of selected systick
 * @param ticks wait for X systicks
 */
void delay(volatile uint32_t ticks)
{
    while (ticks--);
}
/**
 * @brief stop the code execution for x milliseconds
 * @param ms amount of milliseconds to wait
 */
void delay_ms(volatile uint32_t ms)
{
    delay_timer = (SYSTICK_TIMER_SPEED / 1000) * ms;

    // wait until delay_timer was decreased to 0
    while (delay_timer);
}

/**
 * @brief returns the uptime in ms
 * @retval uptime in ms
 */
__inline uint64_t ll_system_get_systime(void)
{
    return systick_counter / 10;
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
