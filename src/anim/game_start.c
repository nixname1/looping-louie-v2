#include <stdlib.h>
#include <stdio.h>

#include "ll_led.h"
#include "ll_anim.h"
#include "anim/game_start.h"

struct payload
{
	uint32_t led_counter;
};

static uint32_t start_animation(struct color *framebuffer, void *payload)
{
	struct payload *p = payload;

	p->led_counter = 0;

    return 1;
}

static uint32_t run_animation(struct color *framebuffer, void *payload)
{
	static uint32_t cnt = 0;
	static uint32_t ring_cnt = 0;
	struct payload *p = payload;
	struct color *col;
	uint32_t player_number = (p->led_counter) / (LL_LED_NUM_STRIPE_PER_PLAYER);
    uint32_t led_index;

	if(p->led_counter >= LL_LED_NUM_STRIPE)
	{
		return 1;
	}

	col = ll_player_get_color(player_number);
	ll_led_set_pixel(framebuffer, col, LL_LED_NUM_ALL_BARS_CIRCLES + p->led_counter);

	*col = WHITE;
	if(p->led_counter % (uint32_t)LL_LED_NUM_STRIPE_PER_PLAYER != 0)
	{
		cnt++;
	}
	else
	{
		cnt = 0;
		ring_cnt = 0;
	}

	if (cnt > 5 && cnt < 12)
	{
        uint32_t offset_up = LL_LED_NUM_PER_BAR + 2;
        uint32_t offset_down = LL_LED_NUM_BAR_CIRCLE_PER_PLAYER - (LL_LED_NUM_PER_CIRCLE - 1);
		ll_led_set_pixel_for_player(framebuffer, col, offset_up + ring_cnt, player_number);

        led_index = offset_down - ring_cnt;
        if(led_index < LL_LED_NUM_PER_BAR)
        {
            led_index += LL_LED_NUM_PER_CIRCLE;
        }
		ll_led_set_pixel_for_player(framebuffer, col, led_index, player_number);
        ring_cnt++;
	}
	p->led_counter++;
	return 0;
}

static uint32_t finish_animation(struct color *framebuffer, void *payload)
{
    return 1;
}

struct animation *anim_game_start_init(struct color *framebuffer)
{
    struct animation      *anim = malloc(sizeof(*anim));
	static struct payload p;

    if (!anim)
        return NULL;

    anim->payload          = &p;
    anim->speed            = 60;
    anim->start_animation  = start_animation;
    anim->run_animation    = run_animation;
    anim->finish_animation = finish_animation;
    anim->framebuffer      = framebuffer;

    return anim;
}
