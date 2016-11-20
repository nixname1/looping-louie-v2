#include "stm32f4xx.h"

#include "diag/Trace.h"

#include "ll_external.h"
#include "ll_system.h"

void EXTI2_IRQHandler(void);
inline static void clock_hc166(void);
uint32_t read_hc166_data(void);

uint32_t mg_exti_oc; // exti2 occured
uint64_t mg_exti_tm; // time when in occured

/**
 * @brief inititalizes the External sources Module
 */
void ll_ext_init(void)
{
    /**
     * initialize control pins for HC166
     */
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
    GPIOC->MODER |= (GPIO_MODER_MODER1_0 | GPIO_MODER_MODER4_0);
    GPIOC->OSPEEDR |= (GPIO_OSPEEDER_OSPEEDR1 & GPIO_OSPEEDER_OSPEEDR4);

    //set PE to HIGH (it is LOW active)
    GPIOC->BSRR = GPIO_BSRR_BS_1;

    /**
    * initialize the EXTI2 Interrupt
    */
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
    SYSCFG->EXTICR[0] = SYSCFG_EXTICR1_EXTI2_PB;
    EXTI->RTSR |= EXTI_RTSR_TR2; // rising trigger
    EXTI->IMR |= EXTI_IMR_MR2; // interrupt mask
    NVIC_EnableIRQ(EXTI2_IRQn);
}

/**
 * @brief handles the external devices
 */
void ll_ext_run()
{
    uint32_t data;

    // if a interrupt occured
    if(mg_exti_oc)
    {
        mg_exti_oc = 0;
        data = read_hc166_data();
        if(data)
        {

        }
    }
}

/**
 * @brief reads the data from HC166
 * @retval  the data read from HC166
 */
uint32_t read_hc166_data(void)
{
    uint32_t data = 0;



    return data;
}

/**
 * @brief give a clock to the HC166
 */
inline static void clock_hc166()
{
    GPIOC->BSRR = GPIO_BSRR_BS_4;
    asm volatile("nop"::);
    asm volatile("nop"::);
    GPIOC->BSRR = GPIO_BSRR_BR_4;
}

/**
 * @brief EXTI2 Handler
 */
void EXTI2_IRQHandler()
{
    EXTI->PR |= EXTI_PR_PR2;
    NVIC_DisableIRQ(EXTI2_IRQn);

    mg_exti_tm = ll_system_get_ticks();
    mg_exti_oc = 1;

    // enable PE so the HC166 saves the actual state
    GPIOC->BSRR = GPIO_BSRR_BR_1;

    clock_hc166();

    // disable PE again, so that the HC166 shifts
    // its data to our DATA IN (PC5)
    GPIOC->BSRR = GPIO_BSRR_BS_1;

    NVIC_EnableIRQ(EXTI2_IRQn);
}
