#include <stdio.h>

#include "stm32f4xx.h"

#include "ll_system.h"
#include "ll_external.h"
#include "ll_reset_switch.h"

uint32_t mg_rs_is_enabled = 0;
uint64_t mg_event_start_time = 0;
uint32_t mg_event_sta

/**
 * @brief initializes the reset switch system
 */
void ll_reset_switch_init()
{
    /** initialize the GPIO Port */
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
    GPIOB->MODER |= GPIO_MODER_MODER6_0;
    GPIOB->OSPEEDR &= ~GPIO_OSPEEDER_OSPEEDR6;

    /** initialize the timer */
    RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;
    TIM4->PSC = 96 - 1;                                // 1000 clocks per ms
    TIM4->ARR = 200 - 1;                               // 200us per signal
    TIM4->CCMR1 = TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1; // pwm mode 1
    TIM4->CCER = TIM_CCER_CC1E;                        // enable ccr

    ll_ext_set_event_callback(LL_EXT_DEVICE_RESET_SWITCH, ll_reset_switch_callback);
}

/**
 * @brief enables the reset switch
 * enable interrupt for switch and enable the LED fade
 */
void ll_reset_switch_enable()
{
    TIM4->CR1 |= TIM_CR1_CEN;
    mg_rs_is_enabled = 1;
}

/**
 * @brief disables the reset switch
 * disable interrupt and LED fade
 */
void ll_reset_switch_disable()
{
    TIM4->CR1 &= ~TIM_CR1_CEN;
    mg_rs_is_enabled = 0;
}

/**
 * @brief returns if the reset switch is enabled or not
 * @return 1 if the RS is enabled; 0 if not
 */
uint32_t ll_reset_switch_is_enabled()
{
    return mg_rs_is_enabled;
}

/**
 * @brief returns if the reset switch is pushed actually or not
 * @return 1 if the reset switch is pushed actually; 0 if not
 */
uint32_t ll_reset_switch_is_pushed()
{
    return ll_ext_is_device_active(LL_EXT_DEVICE_RESET_SWITCH);
}

/**
 * @brief callback for interrupt
 * @param press_length time the switch was pressed
 */
void ll_reset_switch_callback(enum ll_ext_event event)
{
    if( !mg_rs_is_enabled )
    {
        return;
    }

    switch(event)
    {
        case LL_EXT_EVENT_START:
            mg_event_start_time = 0;
            mg_event_end_time = 0;
            mg_event_is_long = 0;
        case LL_EXT_EVENT_END:
    }

}
