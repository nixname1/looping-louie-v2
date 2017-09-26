#ifndef LL_LED_H_
#define LL_LED_H_

#include "ll_player.h"

#define LL_LED_NUM_PER_CIRCLE   (12)
#define LL_LED_NUM_PER_BAR      (8)
#define LL_LED_NUM_PER_PLAYER   (LL_LED_NUM_PER_BAR + LL_LED_NUM_PER_CIRCLE)

#define LL_LED_NUM_LEDS (LL_LED_NUM_PER_PLAYER * LL_PLAYER_MAX_PLAYERS)

#define RED     ((struct color) {255, 0, 0, 255})
#define GREEN   ((struct color) {0, 255, 0, 255})
#define BLUE    ((struct color) {0, 0, 255, 255})
#define YELLOW  ((struct color) {255, 100, 0, 255})
#define PURPLE  ((struct color) {255, 0, 127, 255})
#define WHITE   ((struct color) {255, 255, 255, 255})
#define BLACK   ((struct color) {0, 0, 0, 0})

struct color
{
    uint8_t r; // red
    uint8_t g; // green
    uint8_t b; // blue
	uint8_t a; // alpha
};

void ll_led_init(void);
void ll_led_run(void);
void ll_led_update_all_leds(void);
void ll_led_set_pixel_for_player(struct color *new_pixel, uint32_t pixel_number, uint32_t player);
void ll_led_set_pixel(struct color *color, uint32_t led);
void ll_led_set_alpha_for_player_pixel(uint8_t alpha, uint32_t pixel_number, uint32_t player);
void ll_led_clear_all_pixel(void);
void ll_led_shift_all_left(void);
void ll_led_shift_all_right(void);
void ll_led_shift_player_circle_right(uint32_t player);
#endif
