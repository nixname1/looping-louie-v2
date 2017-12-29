#include <stdio.h>
#include <stdlib.h>

#include "stm32f4xx.h"

#include "diag/Trace.h"

#include "ll_led.h"
#include "ll_lightbarrier.h"
#include "ll_switch.h"
#include "ll_anim.h"
#include "ll_motor.h"
#include "ll_game.h"
#include "ll_player.h"

#include "hardware/ll_renderer.h"
#include "hardware/ll_74hc166.h"
#include "hardware/ll_gpio.h"
#include "hardware/ll_system.h"
#include "hardware/ll_pwm.h"

#include "anim/system_boot.h"
#include "anim/game_start.h"
#include "anim/game_pause.h"
#include "anim/round_start.h"
#include "anim/round_run.h"
#include "anim/round_wait_for_start.h"
#include "anim/player_lost.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunknown-pragmas"

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"

int main(int argc, char *argv[])
{
    UNUSED(argc);
    UNUSED(argv);

    struct color *framebuffer;
	struct game *game = NULL;

#ifdef DEBUG
	trace_printf("SystemCoreClock: %lu\n", SystemCoreClock);
	ll_system_debug_init();
#endif
	ll_system_init();
    ll_system_random_init();
	ll_pwm_init();
    ll_motor_init(ll_pwm_set_speed_in_percent);
    ll_74hc166_init();
    ll_gpio_init();
    ll_switch_init(ll_gpio_get_switch_state);

    framebuffer = ll_led_create_framebuffer();
    ll_renderer_init();
    ll_renderer_render_frame(framebuffer);

    if(!framebuffer)
        return -1;

    ll_anim_init(ll_renderer_render_frame);
    ll_anim_add(LL_ANIM_SYSTEM_BOOT, anim_system_boot_init(framebuffer));
    ll_anim_add(LL_ANIM_GAME_START, anim_game_start_init(framebuffer));
    ll_anim_add(LL_ANIM_GAME_PAUSE, anim_game_pause_init(framebuffer));
    ll_anim_add(LL_ANIM_PLAYER_LOST, anim_player_lost_init(framebuffer));
    ll_anim_add(LL_ANIM_ROUND_START, anim_round_start_init(framebuffer));
    ll_anim_add(LL_ANIM_ROUND_RUN, anim_round_run_init(framebuffer));
    ll_anim_add(LL_ANIM_ROUND_WAIT_FOR_START, anim_round_wait_for_start_init(framebuffer));

    ll_lb_init(ll_74hc166_read_data, ll_game_lb_event_callback);

    while (1)
    {
	    if(game == NULL)
	    {
		    game = ll_game_create();
	    }
	    else
	    {
		    uint32_t ret = ll_game_loop_run(game, ll_system_get_systime());
		    if (ret)
		    {
			    free(game->player);
			    free(game);
			    game = NULL;
		    }
	    }

    }
}
#pragma clang diagnostic pop
#pragma GCC diagnostic pop
