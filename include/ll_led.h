#ifndef LL_LED_H_
#define LL_LED_H_

struct color
{
    uint8_t r; // red
    uint8_t g; // green
    uint8_t b; // blue
};

void ll_led_init(void);
void ll_led_set_pixel(struct color color, uint32_t led);
#endif
