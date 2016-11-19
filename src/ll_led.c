#include <string.h>

#include "stm32f4xx.h"

#include "ll_system.h"
#include "ll_led.h"

static inline void ll_led_write_pixel(struct color color);
static struct color pixel[LL_LED_NUM_LEDS];


/**
 * @brief   initializes the LED system
 * @param   leds_per_player   number of leds for every player
 */
void ll_led_init()
{
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
    GPIOC->MODER |= GPIO_MODER_MODER0_0;
    GPIOC->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR0; // highest speed

    memset(pixel, 0, sizeof(struct color) * (sizeof(pixel) / sizeof(*pixel)) );
}

/**
 * @brief updates the color of all leds
 */
void ll_led_update_all_leds()
{
    uint32_t i = 0;
    while(i < LL_LED_NUM_LEDS)
    {
        ll_led_write_pixel(pixel[i]);
        i++;
    }
}

/**
 * @brief set all leds for a player
 * @param   pixels  the colors; for every led one
 * @param   player  the number of the player to set the leds
 */
void ll_led_set_pixel_for_player(struct color *new_pixel, uint32_t player)
{
    memcpy(&pixel[player * LL_LED_NUM_PER_PLAYER], new_pixel, sizeof(struct color) * LL_LED_NUM_PER_PLAYER);
}

/**
 * @brief shifts all colors to the left side
 */
void ll_led_shift_all_left(void)
{
    uint32_t i;
    struct color tmp;
    tmp = pixel[0];
    for(i = 0; i + 1 < LL_LED_NUM_LEDS ; i++)
    {
        pixel[i] = pixel[i + 1];
    }
    pixel[i] = tmp;
}

/**
 * @brief shifts all colors to the right side
 */
void ll_led_shift_all_right(void)
{
    uint32_t i;
    struct color tmp;
    tmp = pixel[LL_LED_NUM_LEDS - 1];
    for(i = LL_LED_NUM_LEDS - 1; i > 0; i--)
    {
        pixel[i] = pixel[i - 1];
    }
    pixel[i] = tmp;
}
/**
 * @brief   set a single pixel to a specified color
 * @param   pix the     the color to set
 * @param   led_offset  led to set; start counting at 0
 */
void ll_led_set_pixel(struct color color, uint32_t led)
{
    memcpy(&pixel[led], &color, sizeof(struct color));
}
/**
 * @brief   write a single color (24bit) to the LED data output
 * @brief   Format: GRB highest byte first
 * @param   color   the color
 */
static inline void ll_led_write_pixel(struct color color)
{
    uint8_t seq[3] = { color.g, color.r, color.b };
    uint32_t act_seq = 0;
    uint8_t act_bit_cnt;
    uint8_t act_bit = 0;
    uint8_t curbyte;
    __disable_irq();
    while (act_seq < 3)
    {
        curbyte = seq[act_seq];
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
