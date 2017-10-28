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

#include "anim/system_boot.h"
#include "anim/game_start.h"
#include "anim/game_pause.h"
#include "anim/round_run.h"
#include "anim/round_standby.h"
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
	struct game *game;
    struct player *player;

#ifdef DEBUG
	trace_printf("SystemCoreClock: %lu\n", SystemCoreClock);
	ll_system_debug_init();
#endif
	ll_system_init();
    ll_system_rand_init();
    ll_motor_init();
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

    ll_lb_init(ll_74hc166_read_data, ll_game_lb_event_callback);

    player = malloc(sizeof(*player) * LL_PLAYER_NUM_PLAYERS);
    for(uint32_t i = 0; i < LL_PLAYER_NUM_PLAYERS; i++)
    {
        player[i].number = i;
        player[i].chips = 3;
        player[i].color = ll_player_get_color(i);
        player[i].lost_count = 0;
    }
	game = ll_game_create(player, LL_PLAYER_NUM_PLAYERS);

    ll_anim_add(LL_ANIM_PLAYER_LOST, anim_player_lost_init(framebuffer, game));
    ll_anim_add(LL_ANIM_ROUND_RUN, anim_round_run_init(framebuffer, game));
    ll_anim_add(LL_ANIM_ROUND_STANDBY, anim_round_standby_init(framebuffer, game));

    while (1)
    {
	    ll_game_loop_run(game,ll_system_get_systime());
    }
}
#pragma clang diagnostic pop
#pragma GCC diagnostic pop
