#include <stdlib.h>
#include <stdio.h>

#include "ll_led.h"
#include "ll_anim.h"
#include "anim/round_run.h"
#include "anim/round_start.h"
#include "../../../simulation_renderer.h"

#define STEPS ((uint8_t) 30)

struct payload
{
    struct game *game;
    uint8_t step_count;
    uint8_t org_alpha[LL_LED_NUM_LEDS];
	uint8_t alpha_step[LL_LED_NUM_LEDS];
    uint8_t padd1;
};

static void set_initial_led_colors(struct color *framebuffer, struct payload *p)
{
    ll_anim_round_run_generate_colors(framebuffer, p->game);
}

static uint32_t start_animation(struct color *framebuffer, void *payload)
{
    struct payload *p = payload;
    uint32_t i;

    p->step_count = 0;

    set_initial_led_colors(framebuffer, p);
    for(i = 0; i < LL_LED_NUM_LEDS; i++)
    {
        p->org_alpha[i] = framebuffer[i].a;
        p->alpha_step[i] = framebuffer[i].a / STEPS;
        ll_led_set_alpha(framebuffer, 0, i);
    }
    return 1;
}

static uint32_t run_animation(struct color *framebuffer, void *payload)
{
    struct payload *p = payload;
    uint32_t i;

    for(i = 0; i < LL_LED_NUM_LEDS; i++)
    {
        framebuffer[i].a += p->alpha_step[i];
    }

    if(p->step_count >= STEPS)
    {
        return 1;
    }
    p->step_count++;

    return 0;
}

static uint32_t finish_animation(struct color *framebuffer, void *payload)
{
    struct payload *p = payload;
    ll_anim_round_run_generate_colors(framebuffer, p->game);
    return 1;
}

struct animation *anim_round_start_init(struct color *framebuffer, struct game *game)
{
    struct animation      *anim = malloc(sizeof(*anim));
    static struct payload p;

    if (!anim)
        return NULL;

    p.game = game;

    anim->payload           = &p;
    anim->speed             = 60;
    anim->is_loop_animation = 0;
    anim->start_animation   = start_animation;
    anim->run_animation     = run_animation;
    anim->finish_animation  = finish_animation;
    anim->framebuffer       = framebuffer;

    return anim;
}
