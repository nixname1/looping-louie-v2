#ifndef LL_LED_H_
#define LL_LED_H_

#include <stdint.h>
#include "ll_player.h"

#define LL_LED_NUM_PER_CIRCLE            (12)
#define LL_LED_NUM_PER_BAR               (8)
#define LL_LED_NUM_STRIPE                (115)
#define LL_LED_NUM_STRIPE_PER_PLAYER     ((float)LL_LED_NUM_STRIPE/LL_PLAYER_NUM_PLAYERS)
#define LL_LED_NUM_PER_PLAYER            (LL_LED_NUM_PER_BAR + LL_LED_NUM_PER_CIRCLE)
#define LL_LED_NUM_ALL_BARS_CIRCLES      ((LL_LED_NUM_PER_CIRCLE + LL_LED_NUM_PER_BAR) * LL_PLAYER_NUM_PLAYERS)
#define LL_LED_NUM_BAR_CIRCLE_PER_PLAYER (LL_LED_NUM_PER_CIRCLE + LL_LED_NUM_PER_BAR)
#define LL_LED_NUM_LEDS ((LL_LED_NUM_PER_PLAYER * LL_PLAYER_NUM_PLAYERS) + LL_LED_NUM_STRIPE)

#define RED     ((struct color) {255, 0, 0, 255})
#define GREEN   ((struct color) {0, 255, 0, 128})
#define BLUE    ((struct color) {0, 0, 255, 255})
#define YELLOW  ((struct color) {255, 100, 0, 200})
#define PURPLE  ((struct color) {255, 0, 127, 178})
#define WHITE   ((struct color) {255, 255, 255, 128})
#define BLACK   ((struct color) {0, 0, 0, 0})

struct color
{
    uint8_t r; // red
    uint8_t g; // green
    uint8_t b; // blue
	uint8_t a; // alpha
};

struct color *ll_led_create_framebuffer(void);

uint32_t ll_led_stripe_get_start_pos(uint32_t player);
uint32_t ll_led_stripe_get_end_pos(uint32_t player);
uint32_t ll_led_stripe_get_absolute_start_pos(uint32_t player);
uint32_t ll_led_stripe_get_absolute_end_pos(uint32_t player);
uint32_t ll_led_stripe_get_size_for_player(uint32_t player);

void ll_led_shift_all_left(struct color *framebuffer);
void ll_led_shift_all_right(struct color *framebuffer);
void ll_led_shift_player_circle_right(struct color *framebuffer, uint32_t player);
void ll_led_shift_player_bar_down(struct color *framebuffer, uint32_t player);
void ll_led_set_circle_color_for_player(struct color *framebuffer, struct color *col, uint32_t player);
void ll_led_set_pixel_for_player(struct color *framebuffer, struct color *new_pixel, uint32_t pixel_number, uint32_t player);
void ll_led_set_pixel(struct color *framebuffer, struct color *color, uint32_t led);
void ll_led_set_alpha_for_player_pixel(struct color *framebuffer, uint8_t alpha, uint32_t pixel_number, uint32_t player);
void ll_led_clear_pixel_of_player(struct color *framebuffer, uint32_t pixel_number, uint32_t player);
void ll_led_clear_pixel(struct color *framebuffer, uint32_t pixel_number);
void ll_led_clear_all_pixel(struct color *framebuffer);
void ll_led_clear_all_pixel_of_player(struct color *framebuffer, uint32_t player);

void ll_led_stripe_set_pixel(struct color *framebuffer, struct color *new_pixel, uint32_t pos);
void ll_led_stripe_set_pixel_for_player(struct color *framebuffer, struct color *new_pixel, uint32_t pos, uint32_t player);
void ll_led_stripe_set_complete_player(struct color *framebuffer, struct color *col, uint32_t player);
void ll_led_stripe_set_alpha_for_player_pixel(struct color *framebuffer, uint32_t pos, uint8_t alpha, uint32_t player);

#endif
