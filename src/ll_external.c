#include "stm32f4xx.h"

#include "diag/Trace.h"

#include "ll_external.h"
#include "ll_system.h"

inline static void clock_74hc166(void);
inline static void reload_74hc166(void);
uint32_t read_74hc166_data(void);

// the last state of the external devices
uint32_t mg_last_state = 0;
// the time of the last readout
uint64_t mg_last_readout_time = 0;

/**
 * @brief inititalizes the External sources Module
 * Pins:
 * PIN  direction   usage
 * PC1  output      PE (active low at 74hc166)
 * PC4  output      clock
 * PC5  input       data in from 74hc166
 */
void ll_ext_init(void)
{
    /**
     * initialize control pins for 74HC166
     */
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
    GPIOC->MODER |= (GPIO_MODER_MODER1_0 | GPIO_MODER_MODER4_0);
    GPIOC->OSPEEDR |= (GPIO_OSPEEDER_OSPEEDR1 & GPIO_OSPEEDER_OSPEEDR4);

    GPIOC->MODER &= ~(GPIO_MODER_MODER5);
    GPIOC->PUPDR |= GPIO_PUPDR_PUPDR5_1;

    //set PE to HIGH (it is LOW active)
    GPIOC->BSRR = GPIO_BSRR_BS_1;
}

/**
 * @brief handles the external devices
 */
void ll_ext_run()
{
    if (ll_system_get_systime() > mg_last_readout_time + LL_EXT_POLL_TIME)
    {
        mg_last_state = read_74hc166_data();
    }
}

/**
 * @brief adds a callback for a device that is conntected to the 74hc166
 */
void ll_ext_set_callback()
{

}

/**
 * @brief returns the status of a specific device
 * @retval 0 if device was not triggered; 1 if it was
 */
uint32_t ll_ext_is_device_active(uint32_t device)
{
    return (mg_last_state & ((uint32_t) 1 << device)) >> device;
}

/**
 * @brief   retunrns the time of the last device update
 * @retval  timestamp of last update
 */
uint64_t ll_ext_get_last_readout_time(void)
{
    return mg_last_readout_time;
}

/**
 * @brief reads the data from HC166
 * @retval  the data read from HC166
 */
uint32_t read_74hc166_data(void)
{
    uint32_t i;
    uint32_t data = 0;
    uint32_t bit;

    reload_74hc166();

    for (i = 0; i < LL_EXT_DEVICE_COUNT; i++)
    {
        bit = (GPIOC->IDR & GPIO_IDR_IDR_5);
        bit >>= 5;
        data |= bit << (i);
        clock_74hc166();
    }
    mg_last_readout_time = ll_system_get_systime();
    return data;
}

/**
 * @brief reloads the 74hc166 with actual values
 */
inline static void reload_74hc166()
{
    // enables PE (active LOW on 74hc166)
    GPIOC->BSRR = GPIO_BSRR_BR_1;
    // give a clock to 74hc166 so it loads the actual state
    clock_74hc166();
    // disable PE again
    GPIOC->BSRR = GPIO_BSRR_BS_1;
}

/**
 * @brief give a clock to the HC166
 */
inline static void clock_74hc166()
{
    GPIOC->BSRR = GPIO_BSRR_BS_4;
    GPIOC->BSRR = GPIO_BSRR_BR_4;
}
