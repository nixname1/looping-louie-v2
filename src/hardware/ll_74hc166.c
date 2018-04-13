
#include <cmsis/stm32f411xe.h>
#include "stm32f4xx.h"

#include "ll_lightbarrier.h"
#include "hardware/ll_74hc166.h"


/**
 * @brief give a clock to the HC166
 */
inline static void clock_74hc166(void)
{
	GPIOB->BSRR = GPIO_BSRR_BS_13;
	GPIOB->BSRR = GPIO_BSRR_BR_13;
}

/**
 * @brief reloads the 74hc166 with actual values
 */
inline static void reload_74hc166(void)
{
	// enables PE (active LOW on 74hc166)
	GPIOC->BSRR = GPIO_BSRR_BR_1;
	// give a clock to 74hc166 so it loads the actual state
	clock_74hc166();
	// disable PE again
	GPIOC->BSRR = GPIO_BSRR_BS_1;
}

/**
 * @brief reads the data from HC166
 * @retval  the data read from HC166
 */
uint32_t ll_74hc166_read_data()
{
	uint32_t i;
	uint32_t data = 0;
	uint32_t bit;

	reload_74hc166();

	for (i = 0; i < LL_LB_COUNT; i++)
	{
		bit = (GPIOC->IDR & GPIO_IDR_IDR_2);
		bit >>= 2;
		data |= bit << (i);
		clock_74hc166();
	}
	return data;
}

void ll_74hc166_init()
{
	/**
 * initialize control pins for 74HC166
 */
	RCC->AHB1ENR |= (RCC_AHB1ENR_GPIOCEN | RCC_AHB1ENR_GPIOBEN);

	GPIOC->MODER |= GPIO_MODER_MODER1_0;
	GPIOB->MODER |= GPIO_MODER_MODER13_0;

	GPIOC->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR1;
    GPIOB->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR13;

	GPIOC->MODER &= ~(GPIO_MODER_MODER2);
	GPIOC->PUPDR |= GPIO_PUPDR_PUPDR2_1;

	//set PE to HIGH (it is LOW active)
	GPIOC->BSRR = GPIO_BSRR_BS_1;
}