#include <stdlib.h>
#include <stdio.h>

#include "ll_led.h"
#include "ll_anim.h"
#include "anim/round_start.h"

struct payload
{
    struct game *game;
};

static uint32_t start_animation(struct color *framebuffer, void *payload)
{
    struct payload *p = payload;



    return 1;
}

static uint32_t run_animation(struct color *framebuffer, void *payload)
{
    struct payload *p = payload;
    uint32_t i;
    uint32_t threshold_counter = 0;

    for(i = 0; i < p->game->player_count; i++)
    {
        threshold_counter += ll_led_fade_leds_for_player_with_stripe(framebuffer, i, LL_LED_FADE_DIR_IN, 1, 2);
    }

    if(threshold_counter >= LL_LED_NUM_LEDS)
    {
        return 1;
    }
    return 0;
}

static uint32_t finish_animation(struct color *framebuffer, void *payload)
{
    struct payload *p = payload;
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
