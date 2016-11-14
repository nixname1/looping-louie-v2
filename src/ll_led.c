#include "stm32f411xe.h"
#include "ll_system.h"
#include "ll_led.h"

/**
 * @brief   initializes the LED system
 */
void ll_led_init(void)
{
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
    GPIOC->MODER |= GPIO_MODER_MODER0_0;
    GPIOC->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR0; // highest speed
}

/**
 * @brief   set a single pixel to a specified color
 * @param   pix the     the color to set
 * @param   led_offset  led to set starting at 0
 */
void ll_led_set_pixel(struct color color, uint32_t led)
{
    struct color led_off = { 0, 0, 0 };
    while (led > 0)
    {
        ll_led_write_pixel(led_off);
        led--;
    }
    ll_led_write_pixel(color);
}

/**
 * @brief   write a single color (24bit) to the LED data output
 * @brief   Format: GRB highest byte first
 * @param   color   the color
 */
void ll_led_write_pixel(struct color color)
{
    uint8_t seq[3] = { color.g, color.r, color.b };
    uint32_t act_seq = 0;
    uint8_t act_bit = 0;
    uint8_t curbyte;
    while (act_seq < 3)
    {
        curbyte = seq[act_seq];
        while (act_bit < 8)
        {
            curbyte = (uint8_t) (curbyte << 1);
            act_bit = (curbyte & 0x80) >> 7;
            ll_system_debug_led_on();
            ll_system_debug_led_off();
            if(act_bit)
            {
                GPIOC->BSRR = GPIO_BSRR_BS_0;
                GPIOC->BSRR = GPIO_BSRR_BR_0;
            }
            act_bit++;
        }

        act_seq++;
    }
}
