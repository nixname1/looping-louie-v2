#include <stdio.h>

#include "stm32f4xx.h"

#include "diag/Trace.h"

#include "ll_led.h"
#include "ll_external.h"
#include "ll_switch.h"
#include "ll_anim.h"
#include "ll_motor.h"
#include "ll_game.h"

#include "hardware/ll_renderer.h"
#include "hardware/ll_74hc166.h"
#include "hardware/ll_gpio.h"
#include "hardware/ll_system.h"

#include "anim/system_boot.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunknown-pragmas"

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
int main(int argc, char *argv[])
{
    UNUSED(argc);
    UNUSED(argv);

    struct color *framebuffer;
	struct game *game;

#ifdef DEBUG
	trace_printf("SystemCoreClock: %lu\n", SystemCoreClock);
	ll_system_debug_init();
#endif
	ll_system_init();
    ll_system_rand_init();
    ll_motor_init();
    ll_74hc166_init();
    ll_ext_init(ll_74hc166_read_data);
    ll_gpio_init();
    ll_switch_init(ll_gpio_get_switch_state);

    framebuffer = ll_led_create_framebuffer();
    ll_renderer_init();
    ll_renderer_render_frame(framebuffer);

    if(!framebuffer)
        return -1;

    ll_anim_init(ll_renderer_render_frame);
    ll_anim_add(anim_system_boot_init(framebuffer));

	game = ll_game_create();
    while (1)
    {
	    ll_game_loop_run(game,ll_system_get_systime());
    }
}
#pragma clang diagnostic pop
#pragma GCC diagnostic pop