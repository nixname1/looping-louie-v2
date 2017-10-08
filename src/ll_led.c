#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#include "ll_player.h"
#include "ll_led.h"


struct color * ll_led_create_framebuffer()
{
    return calloc(sizeof(struct color), LL_LED_NUM_LEDS);
}

/**
 * @brief shifts all colors to the left side
 */
void ll_led_shift_all_left(struct color *framebuffer)
{
    uint32_t i;
    struct color tmp;
    tmp = framebuffer[0];
    for(i = 0; i + 1 < LL_LED_NUM_LEDS ; i++)
    {
	    framebuffer[i] = framebuffer[i + 1];
    }
	framebuffer[i] = tmp;
}

/**
 * @brief shifts all colors to the right side
 */
void ll_led_shift_all_right(struct color *framebuffer)
{
    uint32_t i;
    struct color tmp;
    tmp = framebuffer[LL_LED_NUM_LEDS - 1];
    for(i = LL_LED_NUM_LEDS - 1; i > 0; i--)
    {
	    framebuffer[i] = framebuffer[i - 1];
    }
	framebuffer[i] = tmp;
}

/**
 * @brief shifts all colors of a circle for a specific player clockwise
 * @param player player to shift circle
 */
void ll_led_shift_player_circle_right(struct color *framebuffer, uint32_t player)
{
    if(player >= LL_PLAYER_NUM_PLAYERS)
        return;

    uint32_t i;
    uint32_t player_offset = (player * LL_LED_NUM_PER_PLAYER);
    uint32_t circle_offset = LL_LED_NUM_PER_BAR;
    uint32_t offset = player_offset + circle_offset;
    struct color tmp;
    tmp = framebuffer[offset];
    for(i = 0; i +1 < LL_LED_NUM_PER_CIRCLE; i++)
    {
	    framebuffer[offset + i] = framebuffer[offset + i + 1];
    }
	framebuffer[offset + i] = tmp;
}

/**
 * @brief shifts all colors of a bar for a specific player from outside to innerside
 * @param
 */
void ll_led_shift_player_bar_down(struct color *framebuffer, uint32_t player)
{
	if (player >= LL_PLAYER_NUM_PLAYERS)
		return;

	uint32_t     i;
	uint32_t     offset = player * LL_LED_NUM_PER_PLAYER;
	struct color tmp    = framebuffer[offset];

	for (i                  = 0; i + 1 < LL_LED_NUM_PER_BAR; i++)
	{
		framebuffer[offset + i] = framebuffer[offset + i + 1];
	}
	framebuffer[offset + i] = tmp;
}

/**
 * @brief set all leds for a player
 * @param   new_pixel   the colors; for every led one
 * @param   player      the number of the player to set the leds for
 * @warning new_pixel has to hold minimum LL_LED_NUM_PER_PLAYER colors!
 */
void ll_led_set_pixel_for_player(struct color *framebuffer, struct color *new_pixel, uint32_t pixel_number, uint32_t player)
{
    memcpy(&framebuffer[player * LL_LED_NUM_PER_PLAYER + pixel_number], new_pixel, sizeof(*framebuffer));
}

/**
 * @brief   set a single pixel to a specified color
 * @param   pix the     the color to set
 * @param   led_offset  led to set; start counting at 0
 */
void ll_led_set_pixel(struct color *framebuffer, struct color *color, uint32_t led)
{
    memcpy(&framebuffer[led], color, sizeof(*framebuffer));
}

/**
 * @brief   sets the alpha value for a specific pixel
 * @param pixel_number  number of the pixel to dimm
 * @param player        player
 */
void ll_led_set_alpha_for_player_pixel(struct color *framebuffer, uint8_t alpha, uint32_t pixel_number, uint32_t player)
{
	framebuffer[player * LL_LED_NUM_PER_PLAYER + pixel_number].a = alpha;
}

/**
 * @brief clears a single pixel of a player
 * @param pixel_number
 * @param player
 */
void ll_led_clear_pixel_of_player(struct color *framebuffer, uint32_t pixel_number, uint32_t player)
{
    memset(&framebuffer[player * LL_LED_NUM_PER_PLAYER + pixel_number], 0, sizeof(*framebuffer));
}

/**
 * @brief clears a single pixel
 * @param pixel_number number of pixel to clear
 */
void ll_led_clear_pixel(struct color *framebuffer, uint32_t pixel_number)
{
    memset(&framebuffer[pixel_number], 0, sizeof(*framebuffer));
}

/**
 * @brief sets all pixel to 0
 */
void ll_led_clear_all_pixel(struct color *framebuffer)
{
    memset(framebuffer, 0, sizeof(*framebuffer) * LL_LED_NUM_LEDS);
}
