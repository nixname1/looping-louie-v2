#include <string.h>

#include "stm32f4xx.h"

#include "ll_system.h"
#include "ll_player.h"
#include "ll_led.h"

static inline void ll_led_write_pixel(struct color color);
static struct color pixel[LL_LED_NUM_LEDS];


/**
 * @brief   initializes the LED system
 */
void ll_led_init()
{
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
    GPIOC->MODER |= GPIO_MODER_MODER0_0;
    GPIOC->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR0; // highest speed

    memset(pixel, 0, sizeof(struct color) * (sizeof(pixel) / sizeof(*pixel)) );

    ll_led_update_all_leds();
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
 * @brief shifts all colors of a circle for a specific player clockwise
 * @param player player to shift circle
 */
void ll_led_shift_player_circle_right(uint32_t player)
{
    if(player >= LL_PLAYER_MAX_PLAYERS)
        return;

    uint32_t i;
    uint32_t player_offset = (player * LL_LED_NUM_PER_PLAYER);
    uint32_t circle_offset = LL_LED_NUM_PER_BAR;
    uint32_t offset = player_offset + circle_offset;
    struct color tmp;
    tmp = pixel[offset];
    for(i = 0; i +1 < LL_LED_NUM_PER_CIRCLE; i++)
    {
        pixel[offset + i] = pixel[offset + i + 1];
    }
    pixel[offset + i] = tmp;
}

/**
 * @brief set all leds for a player
 * @param   new_pixel   the colors; for every led one
 * @param   player      the number of the player to set the leds for
 * @warning new_pixel has to hold minimum LL_LED_NUM_PER_PLAYER colors!
 */
void ll_led_set_pixel_for_player(struct color *new_pixel, uint32_t pixel_number, uint32_t player)
{
    if(player >= LL_PLAYER_MAX_PLAYERS || pixel_number >= LL_LED_NUM_PER_PLAYER)
        return;

    memcpy(&pixel[player * LL_LED_NUM_PER_PLAYER + pixel_number], new_pixel, sizeof(*pixel));
}

/**
 * @brief   set a single pixel to a specified color
 * @param   pix the     the color to set
 * @param   led_offset  led to set; start counting at 0
 */
void ll_led_set_pixel(struct color *color, uint32_t led)
{
    memcpy(&pixel[led], color, sizeof(*pixel));
}

/**
 * @brief   sets the alpha value for a specific pixel
 * @param pixel_number  number of the pixel to dimm
 * @param player        player
 */
void ll_led_set_alpha_for_player_pixel(uint8_t alpha, uint32_t pixel_number, uint32_t player)
{
    if(player >= LL_PLAYER_MAX_PLAYERS || pixel_number >= LL_LED_NUM_PER_PLAYER)
        return;

    struct color *c = &pixel[player * LL_LED_NUM_PER_PLAYER + pixel_number];
    double alpha_d = (double)alpha/(double)255;
    struct color tmp = {c->r*alpha_d, c->g*alpha_d, c->b*alpha_d};
    pixel[player * LL_LED_NUM_PER_PLAYER + pixel_number] = tmp;
}

/**
 * @brief clears a single pixel of a player
 * @param pixel_number
 * @param player
 */
void ll_led_clear_pixel_of_player(uint32_t pixel_number, uint32_t player)
{
    if(player >= LL_PLAYER_MAX_PLAYERS || pixel_number >= LL_LED_NUM_PER_PLAYER)
        return;

    memset(&pixel[player * LL_LED_NUM_PER_PLAYER + pixel_number], 0, sizeof(*pixel));
}

/**
 * @brief clears a single pixel
 * @param pixel_number number of pixel to clear
 */
void ll_led_clear_pixel(uint32_t pixel_number)
{
    memset(&pixel[pixel_number], 0, sizeof(*pixel));
}

/**
 * @brief sets all pixel to 0
 */
void ll_led_clear_all_pixel()
{
    memset(pixel, 0, sizeof(pixel)/sizeof(*pixel));
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
    delay_ms(1);
}

/**
 * @brief   write a single color (24bit) to the LED data output
 * @brief   Format: GRB highest byte first
 * @param   color   the color
 */
static inline void ll_led_write_pixel(struct color color)
{
    uint32_t seq[3] = { color.g, color.r, color.b };
    uint32_t act_seq = 0;
    uint8_t act_bit_cnt;
    uint8_t act_bit = 0;
    uint8_t curbyte;
    __disable_irq();
    while (act_seq < 3)
    {
        curbyte = (uint8_t) (seq[act_seq] * 0.8);
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
