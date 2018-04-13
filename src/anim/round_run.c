#include <stdlib.h>
#include <stdio.h>

#include "ll_led.h"
#include "ll_anim.h"
#include "ll_game.h"
#include "anim/round_run.h"

void ll_anim_round_run_generate_colors(struct color *framebuffer, struct game *game)
{
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

        ll_led_stripe_set_complete_player(framebuffer, ll_player_get_color(i), i);

        for(uint32_t j = 0; j < game->player[i].lost_count; j++)
        {
            ll_led_set_pixel_for_player(framebuffer, ll_player_get_color(i), j, i);
        }
    }
}

static uint32_t start_animation(struct color *framebuffer, struct game *game, int *render_request, void *payload)
{
    (void)(framebuffer);
    (void)(game);
    (void)(render_request);
    (void)(payload);

    return 1;
}

static uint32_t run_animation(struct color *framebuffer, struct game *game, int *render_request, void *payload)
{
	(void)(payload);
    ll_anim_round_run_generate_colors(framebuffer, game);
	*render_request = 1;
    return 1;
}

static uint32_t finish_animation(struct color *framebuffer, struct game *game, int *render_request, void *payload)
{
    (void)(framebuffer);
    (void)(game);
    (void)(render_request);
    (void)(payload);

    return 1;
}

struct animation *anim_round_run_init(struct color *framebuffer)
{
    struct animation      *anim = malloc(sizeof(*anim));

    if (!anim)
        return NULL;

    anim->payload           = NULL;
    anim->speed             = 60;
    anim->is_loop_animation = 1;
    anim->start_animation   = start_animation;
    anim->run_animation     = run_animation;
    anim->finish_animation  = finish_animation;
    anim->framebuffer       = framebuffer;

    return anim;
}


