#ifndef LL_LED_H_
#define LL_LED_H_

#include <stdint.h>
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

typedef void (*ll_led_renderer_render_frame_cb)(struct color *framebuffer);

struct renderer
{
	ll_led_renderer_render_frame_cb render_frame_cb;
};

struct color *ll_led_create_framebuffer(void);
void ll_led_shift_all_left(struct color *framebuffer);
void ll_led_shift_all_right(struct color *framebuffer);
void ll_led_shift_player_circle_right(struct color *framebuffer, uint32_t player);
void ll_led_set_pixel_for_player(struct color *framebuffer, struct color *new_pixel, uint32_t pixel_number, uint32_t player);
void ll_led_set_pixel(struct color *framebuffer, struct color *color, uint32_t led);
void ll_led_set_alpha_for_player_pixel(struct color *framebuffer, uint8_t alpha, uint32_t pixel_number, uint32_t player);
void ll_led_clear_pixel_of_player(struct color *framebuffer, uint32_t pixel_number, uint32_t player);
void ll_led_clear_pixel(struct color *framebuffer, uint32_t pixel_number);
void ll_led_clear_all_pixel(struct color *framebuffer);

#endif
