#include <stdlib.h>
#include <string.h>

#include "ll_led.h"
#include "ll_anim.h"
#include "ll_game.h"

#include "anim/game_pause.h"

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
	uint32_t step_switch_count;
	uint8_t org_alpha[LL_LED_NUM_LEDS + 1];
	uint8_t alpha_step[LL_LED_NUM_LEDS + 1];
	uint8_t brightness_reference;
	uint8_t brightest_value;
	enum step step;
    uint8_t padd1;
};

static uint32_t start_animation(struct color *framebuffer, struct game *game, int *render_request, void *payload)
{
	(void)(game);
	(void)(render_request);
	struct payload *p = payload;
	p->step = STEP_DOWN;
	p->step_count = 0;
	p->step_switch_count = 0;
	p->brightness_reference = 0;
	for (uint32_t i = 0; i < LL_LED_NUM_LEDS; ++i)
	{
		p->org_alpha[i] = framebuffer[i].a;
		p->alpha_step[i] = framebuffer[i].a / (uint8_t) STEP_COUNT;
		if(p->brightest_value < framebuffer[i].a )
		{
			p->brightest_value = framebuffer[i].a;
		}
	}
	p->alpha_step[LL_LED_NUM_LEDS] = p->brightest_value / (uint8_t) STEP_COUNT;
	p->brightness_reference = p->brightest_value;
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
			for (uint32_t i = 0; i < LL_LED_NUM_LEDS; ++i)
			{
				framebuffer[i].a = (uint8_t) (framebuffer[i].a - p->alpha_step[i]);
			}
			p->brightness_reference = (uint8_t) (p->brightness_reference - p->alpha_step[LL_LED_NUM_LEDS]);
			break;

		case STEP_UP:
			p->step_count++;
			for (uint32_t i = 0; i < LL_LED_NUM_LEDS; ++i)
			{
				framebuffer[i].a = (uint8_t) (framebuffer[i].a + p->alpha_step[i]);
			}
			p->brightness_reference = (uint8_t) (p->brightness_reference + p->alpha_step[LL_LED_NUM_LEDS]);
			break;
	}
	*render_request = 1;
	return 1;
}

static uint32_t finish_animation(struct color *framebuffer, struct game *game, int *render_request, void *payload)
{
	(void)(game);

	struct payload *p = payload;
	if (p->brightness_reference == p->brightest_value)
	{
		return 1;
	}

	for (uint32_t i = 0; i < LL_LED_NUM_LEDS; ++i)
	{
		framebuffer[i].a = (uint8_t) (framebuffer[i].a + p->alpha_step[i]);
	}
	p->brightness_reference = (uint8_t) (p->brightness_reference + p->alpha_step[LL_LED_NUM_LEDS]);

	*render_request = 1;
	return 0;
}

struct animation *anim_game_pause_init(struct color *framebuffer)
{
	struct animation      *anim = malloc(sizeof(*anim));
	static struct payload p;

	if (!anim)
		return NULL;

	anim->payload           = &p;
	anim->speed             = 90;
	anim->is_loop_animation = 1;
	anim->start_animation   = start_animation;
	anim->run_animation     = run_animation;
	anim->finish_animation  = finish_animation;
	anim->framebuffer       = framebuffer;

	return anim;
}
