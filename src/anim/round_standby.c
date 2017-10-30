#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include "ll_led.h"
#include "ll_anim.h"
#include "ll_game.h"
#include "anim/round_standby.h"

#define LONG_WAIT (90)
#define SHORT_WAIT (30)

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
    return 1;
}

static uint32_t finish_animation(struct color *framebuffer, void *payload)
{
    return 1;
}

struct animation *anim_round_standby_init(struct color *framebuffer, struct game *game)
{
    struct animation      *anim = malloc(sizeof(*anim));
    static struct payload p;

    if (!anim || !game)
        return NULL;

    p.game = game;

    anim->payload           = &p;
    anim->speed             = 60;
    anim->is_loop_animation = 1;
    anim->start_animation   = start_animation;
    anim->run_animation     = run_animation;
    anim->finish_animation  = finish_animation;
    anim->framebuffer       = framebuffer;

    return anim;
}


