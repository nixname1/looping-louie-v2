#include <stdlib.h>
#include <cmsis/stm32f411xe.h>

#include "stm32f4xx.h"
#include "hardware/ll_system.h"
#include "ll_led.h"
#include "hardware/ll_renderer.h"

static void ll_renderer_render_pixel(struct color *col, volatile uint32_t* reg_addr, uint32_t set_value, uint32_t reset_val);

/**
 * @brief   initializes the LED system
 */
void ll_renderer_init()
{
	RCC->AHB1ENR |= (RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOBEN);
	GPIOA->MODER |= GPIO_MODER_MODER7_0;
	GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR7; // highest speed

	GPIOB->MODER |= GPIO_MODER_MODER5_0;
	GPIOB->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR5;
}

/**
 * @brief updates the color of all leds
 */

void ll_renderer_render_frame(struct color *framebuffer)
{
	uint32_t i = 0;
	for(i = 0; i < LL_LED_NUM_ALL_BARS_CIRCLES; i++)
	{
		ll_renderer_render_pixel(&framebuffer[i], &GPIOA->BSRR, GPIO_BSRR_BS_7, GPIO_BSRR_BR_7);
	}
	for( ; i < LL_LED_NUM_LEDS; i++)
	{
		ll_renderer_render_pixel(&framebuffer[i], &GPIOB->BSRR, GPIO_BSRR_BS_5, GPIO_BSRR_BR_5);
	}
	delay_ms(1);
}

/**
 * @brief   write a single color (24bit) to the LED data output
 * @brief   Format: GRB highest byte first
 * @param   color   the color
 */
static void ll_renderer_render_pixel(struct color *col, volatile uint32_t* reg_addr, uint32_t set_value, uint32_t reset_val)
{
	uint32_t seq[3] = { col->g, col->r, col->b };
	uint32_t act_seq = 0;
	uint8_t act_bit_cnt;
	uint8_t act_bit = 0;
	uint8_t curbyte;
	__disable_irq();
	while (act_seq < 3)
	{
		float factor = (float) col->a / 255;
		curbyte = (uint8_t) (seq[act_seq] * 0.8 * factor);
		act_bit_cnt = 0;
		while (act_bit_cnt < 8)
		{
			act_bit = (curbyte & 0x80) >> 7;
			if(act_bit)
			{
				// send a logical one
				*reg_addr = set_value;
				asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);
				asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);
				asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);
				asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);
				asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);
				asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);
				asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);
				asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);
				*reg_addr = reset_val;
				asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);
				asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);
				asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);
				asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);
				asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);

			}
			else
			{
				// send a logical zero
				*reg_addr = set_value;
				asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);
				asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);
				asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);
				asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);
				asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);
				*reg_addr = reset_val;
				asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);
				asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);
				asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);
				asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);
				asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);
				asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);
				asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);
				asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);

			}
			curbyte = (uint8_t) (curbyte << 1);
			act_bit_cnt++;
		}
		act_seq++;
	}
	__enable_irq();
}
