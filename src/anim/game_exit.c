#include <stdlib.h>
#include <stdio.h>


#include "ll_led.h"
#include "ll_anim.h"
#include "ll_game.h"

#include "anim/round_run.h"
#include "anim/game_exit.h"

#define STEP_COUNT (62)
#define MIN_ALPHA (50)

enum step
{
	STEP_DOWN,
	STEP_UP
};

struct payload
{
	uint32_t step_count;
	uint8_t org_alpha[LL_LED_NUM_STRIPE + 1];
	uint8_t alpha_step[LL_LED_NUM_STRIPE + 1];
	uint8_t brightness_reference;
	uint8_t brightest_value;
	enum step step;
	uint8_t padd1;
};

static uint32_t start_animation(struct color *framebuffer, struct game *game, int *render_request, void *payload)
{
	(void)(framebuffer);
	(void)(game);

	struct payload *p = payload;

	for (uint32_t i = 0; i < game->player_count; ++i)
	{
		uint32_t j;
		for (j = 0; j < game->player[i].lost_count; ++j)
		{
			ll_led_set_pixel_for_player(framebuffer, ll_player_get_color(i), j, i);
		}
		for( j = 0; j < LL_LED_NUM_PER_CIRCLE; j++)
		{
			if(i == game->player_lost->number)
			{
				ll_led_set_pixel_for_player(framebuffer, game->player_lost->color, LL_LED_NUM_PER_BAR + j, i);
			}
			else
			{
				ll_led_set_alpha_for_player_pixel(framebuffer, 0, LL_LED_NUM_PER_BAR + j, i);
			}
		}

		ll_led_stripe_set_complete_player(framebuffer, ll_player_get_color(i), i);
	}

	p->step = STEP_DOWN;
	p->step_count = 0;
	p->brightness_reference = 0;
	for (uint32_t i = 0; i < LL_LED_NUM_STRIPE; ++i)
	{
		p->org_alpha[i] = framebuffer[LL_LED_NUM_ALL_BARS_CIRCLES + i].a;
		p->alpha_step[i] = framebuffer[LL_LED_NUM_ALL_BARS_CIRCLES + i].a / (uint8_t) STEP_COUNT;
		if(p->brightest_value < framebuffer[LL_LED_NUM_ALL_BARS_CIRCLES + i].a )
		{
			p->brightest_value = framebuffer[LL_LED_NUM_ALL_BARS_CIRCLES + i].a;
		}
	}
	p->alpha_step[LL_LED_NUM_STRIPE] = p->brightest_value / (uint8_t) STEP_COUNT;
	p->brightness_reference = p->brightest_value;

	*render_request = 1;
    return 1;
}

static uint32_t run_animation(struct color *framebuffer, struct game *game, int *render_request, void *payload)
{
	(void)(game);

	static uint32_t wait_counter = 0;
	struct payload *p = payload;

	wait_counter++;
	//if(wait_counter < 2)
	{
		//	return 1;
	}
	wait_counter = 0;
	if(p->brightness_reference <= MIN_ALPHA)
	{
		p->step = STEP_UP;
	}
	else if(p->brightness_reference >= p->brightest_value)
	{
		p->step = STEP_DOWN;
	}
	switch(p->step)
	{
		case STEP_DOWN:
			p->step_count++;
			for (uint32_t i = 0; i < LL_LED_NUM_STRIPE; ++i)
			{
				framebuffer[LL_LED_NUM_ALL_BARS_CIRCLES + i].a = (uint8_t) (framebuffer[LL_LED_NUM_ALL_BARS_CIRCLES + i].a - p->alpha_step[i]);
			}
			p->brightness_reference = (uint8_t) (p->brightness_reference - p->alpha_step[LL_LED_NUM_STRIPE]);
			break;

		case STEP_UP:
			p->step_count++;
			for (uint32_t i = 0; i < LL_LED_NUM_STRIPE; ++i)
			{
				framebuffer[LL_LED_NUM_ALL_BARS_CIRCLES + i].a = (uint8_t) (framebuffer[LL_LED_NUM_ALL_BARS_CIRCLES + i].a + p->alpha_step[i]);
			}
			p->brightness_reference = (uint8_t) (p->brightness_reference + p->alpha_step[LL_LED_NUM_STRIPE]);
			break;
	}
	*render_request = 1;
	return 0;
}

static uint32_t finish_animation(struct color *framebuffer, struct game *game, int *render_request, void *payload)
{
	(void)(framebuffer);
	(void) (game);
	(void)(render_request);
	(void)(payload);

    return 1;
}

struct animation *anim_game_exit_init(struct color *framebuffer)
{
    struct animation      *anim = malloc(sizeof(*anim));
	static struct payload p;

    if (!anim)
        return NULL;

    anim->payload           = &p;
    anim->speed             = 60;
	anim->is_loop_animation = 0;
    anim->start_animation   = start_animation;
    anim->run_animation     = run_animation;
    anim->finish_animation  = finish_animation;
    anim->framebuffer       = framebuffer;

    return anim;
}
