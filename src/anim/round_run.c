#include <stdlib.h>
#include <stdio.h>

#include "ll_led.h"
#include "ll_anim.h"
#include "ll_game.h"
#include "anim/round_run.h"

struct payload
{
    struct game *game;
};

static uint32_t start_animation(struct color *framebuffer, void *payload)
{
    return 1;
}

static uint32_t run_animation(struct color *framebuffer, void *payload)
{
    struct payload *p = payload;
    struct game *game = p->game;
    struct color col;
    uint32_t i;
    for(i = 0; i < game->player_count; i++)
    {
        switch(game->player[i].chips)
        {
            case 3:
                col = WHITE;
                break;
            case 2:
                col = GREEN;
                break;
            case 1:
                col = YELLOW;
                break;
            case 0:
                col = RED;
                break;
            default:
                col = BLACK;
        }
        ll_led_set_circle_color_for_player(framebuffer, &col, i);

        for(uint32_t j = 0; j < p->game->player[i].lost_count; j++)
        {
            ll_led_set_pixel_for_player(framebuffer, &RED, j, i);
        }
    }


    return 1;
}

static uint32_t finish_animation(struct color *framebuffer, void *payload)
{
    return 1;
}

struct animation *anim_round_run_init(struct color *framebuffer, struct game *game)
{
    struct animation      *anim = malloc(sizeof(*anim));
    static struct payload p;

    if (!anim || !game)
        return NULL;

    p.game = game;

    anim->payload          = &p;
    anim->speed            = 60;
    anim->start_animation  = start_animation;
    anim->run_animation    = run_animation;
    anim->finish_animation = finish_animation;
    anim->framebuffer      = framebuffer;

    return anim;
}


