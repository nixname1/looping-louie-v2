#include <stdlib.h>

#include "stm32f4xx.h"
#include "ll_system.h"
#include "ll_led.h"
#include "ll_renderer.h"

static void ll_renderer_render_pixel(struct color *col);
static void ll_renderer_render_frame(struct color *framebuffer);

/**
 * @brief   initializes the LED system
 */
struct renderer *ll_renderer_init()
{
	struct renderer *renderer = malloc(sizeof(*renderer));

	if(!renderer)
	{
		return NULL;
	}

	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
	GPIOC->MODER |= GPIO_MODER_MODER0_0;
	GPIOC->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR0; // highest speed

	renderer->render_frame_cb = ll_renderer_render_frame;

	return renderer;
}

/**
 * @brief updates the color of all leds
 */
static void ll_renderer_render_frame(struct color *framebuffer)
{
	uint32_t i = 0;
	while(i < LL_LED_NUM_LEDS)
	{
		ll_renderer_render_pixel(&framebuffer[i]);
		i++;
	}
	delay_ms(1);
}

/**
 * @brief   write a single color (24bit) to the LED data output
 * @brief   Format: GRB highest byte first
 * @param   color   the color
 */
static void ll_renderer_render_pixel(struct color *col)
{
	uint32_t seq[3] = { col->g, col->r, col->b };
	uint32_t act_seq = 0;
	uint8_t act_bit_cnt;
	uint8_t act_bit = 0;
	uint8_t curbyte;
	__disable_irq();
	while (act_seq < 3)
	{
		curbyte = (uint8_t) (seq[act_seq] * 0.8 * col->a);
		act_bit_cnt = 0;
		while (act_bit_cnt < 8)
		{
			act_bit = (curbyte & 0x80) >> 7;
			if(act_bit)
			{
				// send a logical one
				GPIOC->BSRR = GPIO_BSRR_BS_0;
				asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);
				asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);
				asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);
				asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);
				asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);
				asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);
				asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);
				asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);
				GPIOC->BSRR = GPIO_BSRR_BR_0;
				asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);
				asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);
				asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);
				asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);
				asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);

			}
			else
			{
				// send a logical zero
				GPIOC->BSRR = GPIO_BSRR_BS_0;
				asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);
				asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);
				asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);
				asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);
				asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);asm volatile("nop"::);
				GPIOC->BSRR = GPIO_BSRR_BR_0;
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
