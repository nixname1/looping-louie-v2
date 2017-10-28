#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#include "ll_player.h"
#include "ll_led.h"

struct stripe_pos
{
	uint32_t start;
	uint32_t end;
};
static struct stripe_pos stripe_positions[LL_PLAYER_NUM_PLAYERS] =
		                        {
				                        {0, 14},
				                        {15, 28},
				                        {29, 43},
				                        {44, 57},
				                        {58, 72},
				                        {73, 86},
				                        {87, 101},
				                        {102, 114}
		                        };
struct color * ll_led_create_framebuffer()
{
    return calloc(sizeof(struct color), LL_LED_NUM_LEDS);
}

uint32_t ll_led_stripe_get_start_pos(uint32_t player)
{
	return stripe_positions[player].start;
}

uint32_t ll_led_stripe_get_end_pos(uint32_t player)
{
	return stripe_positions[player].end;
}

uint32_t ll_led_stripe_get_absolute_start_pos(uint32_t player)
{
    return LL_LED_NUM_ALL_BARS_CIRCLES + ll_led_stripe_get_start_pos(player);
}

uint32_t ll_led_stripe_get_absolute_end_pos(uint32_t player)
{
    return LL_LED_NUM_ALL_BARS_CIRCLES + ll_led_stripe_get_end_pos(player);
}

uint32_t ll_led_stripe_get_size_for_player(uint32_t player)
{
	return (ll_led_stripe_get_end_pos(player) - ll_led_stripe_get_start_pos(player)) + 1;
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

void ll_led_set_circle_color_for_player(struct color *framebuffer, struct color *col, uint32_t player)
{
    for(uint32_t i = 0; i < LL_LED_NUM_PER_CIRCLE; i++)
    {
        memcpy(&framebuffer[player * LL_LED_NUM_PER_PLAYER + LL_LED_NUM_PER_BAR + i], col, sizeof(*framebuffer));
    }
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

void ll_led_clear_all_pixel_of_player(struct color *framebuffer, uint32_t player)
{
    memset(&framebuffer[LL_LED_NUM_PER_PLAYER * player], 0, sizeof(*framebuffer) * LL_LED_NUM_PER_PLAYER);
}

uint32_t ll_led_fade_leds_for_player_with_stripe(struct color *framebuffer, uint32_t player, enum LL_LED_FADE_DIR dir, float percent, uint8_t zero_threshold)
{
    uint32_t zero_counter = 0;
    uint32_t i;

    zero_counter += ll_led_fade_leds_for_player(framebuffer, player, dir, percent, zero_threshold);
    uint32_t player_stripe_len = ll_led_stripe_get_size_for_player(player);
    uint32_t offset = ll_led_stripe_get_absolute_start_pos(player);

    for(i = 0; i < player_stripe_len; i++)
    {
        zero_counter += ll_led_fade_pixel(framebuffer, offset + i, dir, percent, zero_threshold);
    }

    return zero_counter;
}

uint32_t ll_led_fade_leds_for_player(struct color *framebuffer, uint32_t player, enum LL_LED_FADE_DIR dir, float percent, uint8_t zero_threshold)
{
    uint32_t i;
    uint32_t zero_counter = 0;
    for (i = 0; i < LL_LED_NUM_PER_PLAYER; i++)
    {
        zero_counter += ll_led_fade_pixel(framebuffer, LL_LED_NUM_PER_PLAYER * player + i, dir, percent, zero_threshold);
    }
    return zero_counter;
}

uint32_t ll_led_fade_leds(struct color *framebuffer, enum LL_LED_FADE_DIR dir, float percent, uint8_t zero_threshold)
{
    uint32_t i;
    uint32_t zero_counter = 0;
    for(i = 0; i < LL_LED_NUM_LEDS; i++)
    {
        zero_counter += ll_led_fade_pixel(framebuffer, i, dir, percent, zero_threshold);
    }
    return zero_counter;
};

uint32_t ll_led_fade_pixel(struct color *framebuffer, uint32_t pixel, enum LL_LED_FADE_DIR dir, float percent, uint8_t zero_threshold)
{
    struct color *c = &framebuffer[pixel];
    if(dir == LL_LED_FADE_DIR_OUT)
    {
        c->a *= 1 - (percent / 100);
        if (c->a < zero_threshold)
        {
            ll_led_clear_pixel(framebuffer, pixel);
            return 1;
        }
    }
    else
    {
        c->a *= 1 + (percent / 100);
        if(c->a > zero_threshold)
        {
            c->a = 255;
            ll_led_set_pixel(framebuffer, c, pixel);
            return 1;
        }
    }

    ll_led_set_pixel(framebuffer, c, pixel);

    return 0;
}

/**
 * @brief sets a sepcific pixel on the stripe
 * @param framebuffer
 * @param new_pixel
 */
void ll_led_stripe_set_pixel(struct color *framebuffer, struct color *new_pixel, uint32_t pos)
{
	framebuffer[LL_LED_NUM_ALL_BARS_CIRCLES + pos] = *new_pixel;
}

void ll_led_stripe_set_pixel_for_player(struct color *framebuffer, struct color *new_pixel, uint32_t pos, uint32_t player)
{
	framebuffer[LL_LED_NUM_ALL_BARS_CIRCLES + ll_led_stripe_get_start_pos(player) + pos] = *new_pixel;
}

void ll_led_stripe_set_complete_player(struct color *framebuffer, struct color *col, uint32_t player)
{
	uint32_t len = ll_led_stripe_get_end_pos(player) - ll_led_stripe_get_start_pos(player);
	for(uint32_t i = 0; i <= len; i++)
	{
		ll_led_stripe_set_pixel_for_player(framebuffer, col, i, player);
	}
}

void ll_led_stripe_set_alpha_for_player_pixel(struct color *framebuffer, uint32_t pos, uint8_t alpha, uint32_t player)
{
	framebuffer[LL_LED_NUM_ALL_BARS_CIRCLES + ll_led_stripe_get_start_pos(player) + pos].a = alpha;
}