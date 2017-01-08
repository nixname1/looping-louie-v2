#include "stm32f4xx.h"
#include "ll_anim_game_start.h"
#include "ll_anim.h"
#include "ll_player.h"
#include "ll_led.h"

struct payload
{
    uint8_t dir;
    uint8_t delay_bars;
    uint8_t delay_circ;
    uint8_t brightest;
    uint8_t middle;
    uint8_t darkest;
    enum player player;
};

uint32_t finish_animation(void *payload);

uint32_t run_animation(void *payload);

uint32_t start_animation(void *payload);

/**
 * @brief finish callback for this animation
 * @param payload payload
 * @return 1 if finished, 0 if not yet
 */
uint32_t finish_animation(void *payload)
{
    uint32_t i, j;
    struct payload *p = payload;
    for (i = 0; i < p->player; i++)
    {
        for (j = 0; j < LL_LED_NUM_PER_PLAYER; j++)
        {
            ll_led_set_alpha_for_player_pixel(255 / ((LL_LED_NUM_PER_PLAYER - (j + 1)) + 1), j, i);
        }
    }

    return 1;
}

/**
 * @brief run callback for this animation
 * @param payload payload
 * @return 1 if finished, 0 if not
 */
uint32_t run_animation(void *payload)
{
    uint32_t i, j;
    struct payload *p = payload;
    struct color *color;
    struct color no_color = {0, 0, 0};

    if (p->delay_bars >= 2)
    {
        switch (p->dir)
        {
            case 0: /* down */
                // first step
                if (p->darkest == 0 && p->brightest == 1)
                {
                    /* expand middle pixel */
                    p->middle = p->brightest;
                    p->brightest++;
                } else if (p->brightest < LL_LED_NUM_PER_BAR - 1 && p->brightest > p->middle && p->middle > p->darkest)
                {
                    /* just walk down */
                    p->darkest++;
                    p->middle++;
                    p->brightest++;
                } else if (p->brightest == LL_LED_NUM_PER_BAR - 1 && p->middle == p->brightest - 1)
                {
                    /* remove middle - reduce to brightest and darkest onyl*/
                    p->darkest++;
                    /* set it to off by one - so this means "do not show" */
                    p->middle = LL_LED_NUM_PER_BAR;
                } else if (p->brightest == LL_LED_NUM_PER_BAR - 1 && p->middle == LL_LED_NUM_PER_BAR)
                {
                    /* switch brightest and darkest */
                    p->darkest++;
                    p->brightest--;
                    /* switch direction */
                    p->dir ^= 1;
                }
                break;

            case 1: /* up */
                if (p->darkest == LL_LED_NUM_PER_BAR - 1 && p->brightest == p->darkest - 1)
                {
                    /* expand middle pixel */
                    p->middle = p->brightest;
                    p->brightest--;
                } else if (p->brightest > 0 && p->darkest > p->middle && p->middle > p->brightest)
                {
                    /* just walk*/
                    p->darkest--;
                    p->middle--;
                    p->brightest--;
                } else if (p->brightest == 0 && p->middle < p->darkest)
                {
                    /* remove middle - reduce to darkest and brightest onyl*/
                    p->darkest--;
                    /* set it to off by one - so this means "do not show" */
                    p->middle = LL_LED_NUM_PER_BAR;
                } else if (p->brightest == 0 && p->middle == LL_LED_NUM_PER_BAR)
                {
                    /* switch brightest and darkest */
                    p->darkest--;
                    p->brightest++;
                    /* switch direction */
                    p->dir ^= 1;
                }
                break;

            default:
                p->dir = 0;
        }
        for (i = 0; i < p->player; i++)
        {
            color = ll_player_get_color_by_int(i);

            for (j = 0; j < LL_LED_NUM_PER_BAR; j++)
            {
                if (j == p->brightest)
                    ll_led_set_pixel_for_player(*color, j, i);
                else if (j == p->middle)
                {
                    ll_led_set_pixel_for_player(*color, j, i);
                    ll_led_set_alpha_for_player_pixel(127, j, i);
                } else if (j == p->darkest)
                {
                    ll_led_set_pixel_for_player(*color, j, i);
                    ll_led_set_alpha_for_player_pixel(45, j, i);
                } else
                {
                    ll_led_set_pixel_for_player(no_color, j, i);
                }
            }
        }
        p->delay_bars = 0;
    } else
    {
        p->delay_bars++;
    }

    if (p->delay_circ >= 2)
    {
        for (i = 0; i < p->player; i++)
        {
            ll_led_shift_player_circle_right(i);
        }
        p->delay_circ = 0;
    } else
        p->delay_circ++;

    return 1;
}

/**
 * @brief start callback for animation
 * @param payload payload
 * @return 1 if finished, 0 if not yet
 */
uint32_t start_animation(void *payload)
{
    uint32_t i, j;
    uint8_t alpha;
    struct payload *p = payload;
    for (i = 0; i < p->player; i++)
    {
        struct color *c = ll_player_get_color_by_int(i);
        for (j = 0; j < LL_LED_NUM_PER_CIRCLE / 2; j++)
        {
            alpha = ((double) 255 / (LL_LED_NUM_PER_CIRCLE / 2)) * (j + 1);
            ll_led_set_pixel_for_player(*c, LL_LED_NUM_PER_BAR + j, i);
            ll_led_set_alpha_for_player_pixel(alpha, LL_LED_NUM_PER_BAR + j, i);

            ll_led_set_pixel_for_player(*c, LL_LED_NUM_PER_BAR + (LL_LED_NUM_PER_CIRCLE - j - 1), i);
            ll_led_set_alpha_for_player_pixel(alpha, LL_LED_NUM_PER_BAR + (LL_LED_NUM_PER_CIRCLE - j - 1), i);
        }

        ll_led_set_pixel_for_player(*c, 0, i);
        ll_led_set_alpha_for_player_pixel(45, 0, i);
        //ll_led_set_pixel_for_player(*c, 1, i);
        //ll_led_set_alpha_for_player_pixel(127, 1, i);
        ll_led_set_pixel_for_player(*c, 1, i);
    }
    p->dir = 0;
    p->brightest = 1;
    p->middle = LL_LED_NUM_PER_BAR; // means "not shown"
    p->darkest = 0;

    return 1;
}

/**
 * @brief animate the boot animation
 */
void ll_anim_animate_wait_for_game_start(void)
{
    struct animation boot;
    static struct payload p;
    p.player = LL_PLAYER_ALL;
    boot.speed = 18;
    boot.payload = &p;
    boot.start_animation = start_animation;
    boot.run_animation = run_animation;
    boot.finish_animation = finish_animation;

    ll_anim_activate(&boot);
}
