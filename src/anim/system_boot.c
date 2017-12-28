#include <stdlib.h>

#include "ll_led.h"
#include "ll_anim.h"
#include "ll_game.h"

#include "anim/system_boot.h"

struct payload
{
    uint32_t bar_cnt;
    uint32_t ring_cnt;
};
uint32_t start_animation(struct color *framebuffer, struct game *game, void *payload);
uint32_t run_animation(struct color *framebuffer, struct game *game, void *payload);
uint32_t finish_animation(struct color *framebuffer, struct game *game, void *payload);

static void set_initial_led_colors(struct color *framebuffer)
{
	uint8_t i, j;
	uint8_t factor;

	for (i = 0; i < LL_PLAYER_NUM_PLAYERS; i++)
	{
		struct color *c = ll_player_get_color(i);
		for (j = 0; j < LL_LED_NUM_PER_CIRCLE / 2; j++)
		{
			factor = 255 / (LL_LED_NUM_PER_CIRCLE / 2);
			c->a = (uint8_t) (factor * (j + (uint8_t) 1));

			ll_led_set_pixel_for_player(framebuffer, c, (uint32_t) LL_LED_NUM_PER_BAR + j, i);
			ll_led_set_pixel_for_player(framebuffer, c, (uint32_t) LL_LED_NUM_PER_BAR +
			                            ((uint32_t) LL_LED_NUM_PER_CIRCLE - j - 1), i);
		}

		factor = 255 / (LL_LED_NUM_PER_BAR);
		for (j = 0; j < LL_LED_NUM_PER_BAR; j++)
		{
			c->a = (uint8_t) (factor * (j + (uint8_t) 1));
			ll_led_set_pixel_for_player(framebuffer, c, j, i);
		}

		ll_led_stripe_set_complete_player(framebuffer, &BLACK, i);
	}
}

/**
 * @brief start callback for animation
 * @param payload payload
 * @return 1 if finished, 0 if not yet
 */
uint32_t start_animation(struct color *framebuffer, struct game *game, void *payload)
{
	(void)(game);

	struct payload *p = payload;

    p->bar_cnt = 0;
    p->ring_cnt = 0;
	ll_led_clear_all_pixel(framebuffer);
	set_initial_led_colors(framebuffer);

	return 1;
}

/**
 * @brief run callback for this animation
 * @param payload payload
 * @return 1 if finished, 0 if not
 */
uint32_t run_animation(struct color *framebuffer, struct game *game, void *payload)
{
	(void)(game);

    struct payload *p = payload;
	uint32_t i;

    p->ring_cnt++;
    p->bar_cnt++;

    if(p->ring_cnt >= 2)
    {
        for (i = 0; i < LL_PLAYER_NUM_PLAYERS; i++)
        {
            ll_led_shift_player_circle_right(framebuffer, i);
        }
        p->ring_cnt = 0;
    }
    if(p->bar_cnt >= 3)
    {
        for (i = 0; i < LL_PLAYER_NUM_PLAYERS; i++)
        {
            ll_led_shift_player_bar_down(framebuffer, i);
        }
        p->bar_cnt = 0;
    }
	return 1;
}

/**
 * @brief finish callback for this animation
 * @param payload payload
 * @return 1 if finished, 0 if not yet
 */
uint32_t finish_animation(struct color *framebuffer, struct game *game, void *payload)
{
	uint32_t ret = 0;
	uint32_t i;
	uint32_t zero_counter = 0;

	// add fading out
	for(i = 0; i < LL_PLAYER_NUM_PLAYERS; i++)
	{
		zero_counter += ll_led_fade_leds_for_player(framebuffer, i, LL_LED_FADE_DIR_OUT, 5, 3);
	}
	if(zero_counter >= LL_LED_NUM_ALL_BARS_CIRCLES)
	{
		return 1;
	}
	// keep the animation running
	run_animation(framebuffer, game, payload);

	return ret;
}

struct animation *anim_system_boot_init(struct color *framebuffer)
{
	struct animation      *anim = malloc(sizeof(*anim));
	static struct payload p;
	if (!anim)
		return NULL;

	anim->payload           = &p;
	anim->speed             = 60;
    anim->is_loop_animation = 1;
	anim->start_animation   = start_animation;
	anim->run_animation     = run_animation;
	anim->finish_animation  = finish_animation;
	anim->framebuffer       = framebuffer;

	return anim;
}