#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include "ll_led.h"
#include "ll_anim.h"
#include "ll_game.h"
#include "anim/player_lost.h"

#define LONG_WAIT (60)
#define SHORT_WAIT (15)

enum step
{
    STEP_FADE,
    STEP_BLINK
};

struct payload
{
    struct game *game;
    uint32_t player_lost;
    uint32_t zero_counter;
    enum step run_step;
};

static void fade_leds(struct color *framebuffer, struct payload *p)
{
    uint32_t i;
    struct color *c;

    for (i = 0; i < LL_LED_NUM_LEDS; i++)
    {
        if(
                i >= ll_led_stripe_get_absolute_start_pos(p->player_lost) &&
                i <= ll_led_stripe_get_absolute_end_pos(p->player_lost))
        {
            continue;
        }
        else if(
                i >= LL_LED_NUM_PER_PLAYER * p->player_lost &&
                i <= LL_LED_NUM_PER_PLAYER * p->player_lost + LL_LED_NUM_PER_PLAYER )
        {
            continue;
        }

        c = &framebuffer[i];
        c->a *= 0.85;
        if (c->a < 3 && c->a > 0)
        {
            p->zero_counter++;
            framebuffer[i].a = 0;
        } else
        {
            ll_led_set_pixel(framebuffer, c, i);
        }
    }
}

static void player_switch(struct color *framebuffer, struct payload *p, uint32_t type)
{
    uint8_t alpha;

    if(type)
    {
        alpha = RED.a;
    }
    else
    {
        alpha = 0;
    }

    ll_led_set_alpha_for_player_pixel(framebuffer, alpha, p->game->player[p->player_lost].lost_count - 1, p->player_lost);
    for(uint32_t i = 0; i < LL_LED_NUM_PER_CIRCLE; i++)
    {
        ll_led_set_alpha_for_player_pixel(framebuffer, alpha, LL_LED_NUM_PER_BAR + i, p->player_lost);
    }

    for(uint32_t i = 0; i < ll_led_stripe_get_size_for_player(p->player_lost); i++)
    {
        ll_led_stripe_set_alpha_for_player_pixel(framebuffer, i, alpha, p->player_lost);
    }
}

static void set_initial_led_colors(struct color *framebuffer, struct payload *p)
{
    ll_led_set_pixel_for_player(framebuffer, &RED, p->game->player[p->player_lost].lost_count - 1, p->player_lost);
    ll_led_set_circle_color_for_player(framebuffer, &RED, p->player_lost);
    ll_led_stripe_set_complete_player(framebuffer, &RED, p->player_lost);
}

static uint32_t start_animation(struct color *framebuffer, void *payload)
{
    struct payload *p = payload;
    uint32_t i;

    for(i = 0; i < p->game->player_count; i++)
    {
        if(p->game->player[i].chips == 0)
        {
            p->player_lost = i;
            break;
        }
    }

    set_initial_led_colors(framebuffer, p);

    p->run_step = STEP_FADE;
    p->zero_counter = 0;
    for(i = 0; i < LL_LED_NUM_LEDS; i++)
    {
        if(framebuffer[i].a < 3)
        {
            p->zero_counter++;
        }
    }

    return 1;
}

static uint32_t run_animation(struct color *framebuffer, void *payload)
{
    static uint32_t blink_state = 0;
    static uint32_t blink_step = 1;
    static uint32_t wait_tm = LONG_WAIT;
    struct payload *p = payload;

    switch(p->run_step)
    {
        case STEP_FADE:
            fade_leds(framebuffer, p);
            if (p->zero_counter >= (LL_LED_NUM_LEDS - LL_LED_NUM_PER_PLAYER - LL_LED_NUM_STRIPE_PER_PLAYER))
            {
                blink_state = 1;
                p->run_step = STEP_BLINK;
            }
            break;

        case STEP_BLINK:
            if(wait_tm)
            {
                wait_tm--;
                return (blink_state) ? 0: 1;
            }
            switch(blink_step)
            {
                case 1:
                    player_switch(framebuffer, p, 0);
                    blink_state = 0;
                    wait_tm = SHORT_WAIT;
                    break;

                case 2:
                    player_switch(framebuffer, p, 1);
                    blink_state = 1;
                    wait_tm = SHORT_WAIT;
                    break;

                case 3:
                    player_switch(framebuffer, p, 0);
                    blink_state = 0;
                    wait_tm = SHORT_WAIT;
                    break;

                case 4:
                    player_switch(framebuffer, p, 1);
                    blink_state = 1;
                    wait_tm = SHORT_WAIT;
		            break;

	            case 5:
                    player_switch(framebuffer, p, 0);
                    blink_state = 0;
                    wait_tm = SHORT_WAIT;
                    break;

                case 6:
                    player_switch(framebuffer, p, 1);
                    blink_state = 1;
                    wait_tm = LONG_WAIT;
                    blink_step = 0;
                    break;

                default:
                    blink_state = 0;
                    blink_step = 0;
                    wait_tm = 0;
            }
            blink_step++;
            break;
    }
    return blink_state;
}

static uint32_t finish_animation(struct color *framebuffer, void *payload)
{
    struct payload *p = payload;
    ll_led_clear_all_pixel_of_player(framebuffer, p->player_lost);
    return 1;
}

struct animation *anim_player_lost_init(struct color *framebuffer, struct game *game)
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


