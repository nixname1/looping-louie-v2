#include <stdlib.h>

#include "ll_led.h"
#include "ll_anim.h"
#include "anim/game_start.h"

static uint32_t start_animation(struct color *framebuffer, void *payload)
{
    return 1;
}

static uint32_t run_animation(struct color *framebuffer, void *payload)
{
    return 1;
}

static uint32_t finish_animation(struct color *framebuffer, void *payload)
{
    return 1;
}

struct animation *anim_game_start_init(struct color *framebuffer)
{
    struct animation      *anim = malloc(sizeof(*anim));

    if (!anim)
        return NULL;

    anim->payload          = NULL;
    anim->speed            = 1000/60;
    anim->start_animation  = start_animation;
    anim->run_animation    = run_animation;
    anim->finish_animation = finish_animation;
    anim->framebuffer      = framebuffer;

    return anim;
}