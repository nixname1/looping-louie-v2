#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include "ll_led.h"
#include "ll_anim.h"
#include "ll_game.h"
#include "anim/round_standby.h"

#define LONG_WAIT (90)
#define SHORT_WAIT (30)

static uint32_t start_animation(struct color *framebuffer, struct game *game, void *payload)
{
    (void)(framebuffer);
    (void)(payload);

	for (uint32_t i = 0; i < game->player_count; ++i)
	{
		for (uint32_t j = 0; j < game->player[i].lost_count; ++j)
		{
			ll_led_set_pixel_for_player(framebuffer, ll_player_get_color(i), j, i);
		}
	}

	return 1;
}

static uint32_t run_animation(struct color *framebuffer, struct game *game, void *payload)
{
    (void)(framebuffer);
    (void)(game);
    (void)(payload);

    return 1;
}

static uint32_t finish_animation(struct color *framebuffer, struct game *game, void *payload)
{
    (void)(framebuffer);
    (void)(game);
    (void)(payload);

    return 1;
}

struct animation *anim_round_standby_init(struct color *framebuffer)
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


