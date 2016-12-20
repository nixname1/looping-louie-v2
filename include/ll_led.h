#ifndef LL_LED_H_
#define LL_LED_H_

#include "ll_player.h"

#ifdef DEBUG
#define LL_LED_NUM_PER_PLAYER   20 // just for debugging purposes
#else
#define LL_LED_NUM_PER_PLAYER   20
#endif

#define LL_LED_NUM_LEDS (LL_LED_NUM_PER_PLAYER * LL_SYSTEM_NUM_PLAYERS)

struct color
{
    uint8_t r; // red
    uint8_t g; // green
    uint8_t b; // blue
};

void ll_led_init(void);
void ll_led_run(void);
void ll_led_update_all_leds(void);
void ll_led_set_pixel_for_player(struct color *new_pixel, uint32_t player);
void ll_led_set_pixel(struct color color, uint32_t led);
void ll_led_clear_all_pixel(void);
void ll_led_shift_all_left(void);
void ll_led_shift_all_right(void);
#endif
