#include <stdlib.h>

#include "ll_anim.h"
#include "ll_switch.h"
#include "ll_lightbarrier.h"
#include "ll_game.h"

enum ll_system_step
{
	LL_SYSTEM_STEP_BOOT,
	LL_SYSTEM_STEP_GAME_START,
	LL_SYSTEM_STEP_GAME_RUN,
	LL_SYSTEM_STEP_GAME_PAUSE,
	LL_SYSTEM_STEP_GAME_EXIT,
	LL_SYSTEM_STEP_STANDBY,
	LL_SYSTEM_STEP_SHUTDOWN,
	LL_SYSTEM_STEP_ERROR
};

enum boot_step
{
	LL_BOOT_STEP_INIT,
	LL_BOOT_STEP_WAIT_FOR_SWITCH,
	LL_BOOT_STEP_WAIT_FOR_ANIMATION
};

static uint32_t run_system_boot(uint64_t systime)
{
	static enum boot_step step = LL_BOOT_STEP_INIT;
	static uint64_t boot_time = 0;

	switch(step)
	{
		case LL_BOOT_STEP_INIT:
			ll_anim_activate(LL_ANIM_SYSTEM_BOOT);
			boot_time = systime;
			step = LL_BOOT_STEP_WAIT_FOR_SWITCH;
			break;

		case LL_BOOT_STEP_WAIT_FOR_SWITCH:
			if(systime >= boot_time + 3000 && !ll_switch_is_turned_on())
			{
				ll_anim_stop_animation();
				step = LL_BOOT_STEP_WAIT_FOR_ANIMATION;
			}
			break;

		case LL_BOOT_STEP_WAIT_FOR_ANIMATION:
			if(!ll_anim_is_active())
			{
				step = LL_BOOT_STEP_INIT;
				return 1;
			}
			break;
	}
	return 0;
}

static uint32_t start_new_round(struct game *game)
{
    uint32_t ret = 0;

    if(!ll_anim_is_active())
    {
        ret = 1;
    }

    return ret;
}

static uint32_t run_round(struct game *game)
{
    uint32_t ret = 0;

    return ret;
}

static uint32_t end_round(struct game *game)
{
    uint32_t ret = 0;

    return ret;
}

void ll_game_lb_event_callback(enum ll_lb_event_type event, uint32_t lightbarrier, uint64_t event_time, void *payload)
{
    struct game *game = payload;
	static uint64_t event_start_time;
	static uint64_t event_end_time;

	if(event == LL_EXT_EVENT_START)
	{
		event_start_time = event_time;
	}
	else if(event == LL_EXT_EVENT_END)
	{
		event_end_time = event_time;
		if(game->player[lightbarrier].chips > 0)
		{
            game->player[lightbarrier].chips--;
		}
	}
}

struct game *ll_game_create(struct player *player, uint32_t player_count)
{
	struct game *game = malloc(sizeof(*game));
	if(!game)
	{
		return NULL;
	}
	game->state = LL_GAME_STATE_STOPPED;
	game->motor_speed = 0;
	game->player = player;
	game->player_count = player_count;
	return game;
}

uint32_t ll_game_start(struct game *game)
{
    uint32_t ret = 0;
	game->state = LL_GAME_STATE_STARTING;
    ll_anim_activate(LL_ANIM_GAME_START);
    return ret;
}

uint32_t ll_game_run(struct game *game)
{
    uint32_t ret = 0;
    uint32_t round_ret = 0;
	game->state = LL_GAME_STATE_RUNNING;

    switch(game->round_step)
    {
        case LL_ROUND_STEP_START:
            if(start_new_round(game))
            {
                game->round_step = LL_ROUND_STEP_RUN;
            }
            break;

        case LL_ROUND_STEP_RUN:
            round_ret = run_round(game);
            if(round_ret)
            {
                game->round_step = LL_ROUND_STEP_END;
            }
            break;

        case LL_ROUND_STEP_END:
            if(end_round(game))
            {
                ret = 1;
            }
            break;
    }

    return ret;
}

void ll_game_pause(struct game *game)
{
	game->state = LL_GAME_STATE_PAUSED;
}

void ll_game_stop(struct game *game)
{
	game->state = LL_GAME_STATE_STOPPING;
}

void ll_game_loop_run(struct game *game, uint64_t systime)
{
	static enum ll_system_step actual_system_step = LL_SYSTEM_STEP_BOOT;

	ll_lb_run(systime, game);
	ll_anim_run(systime);

	switch (actual_system_step)
	{
		case LL_SYSTEM_STEP_BOOT:
			if(run_system_boot(systime))
				actual_system_step = LL_SYSTEM_STEP_GAME_START;
			break;

		case LL_SYSTEM_STEP_GAME_START:
			// TODO: print game start animation
			if(ll_game_start(game))
            {
                actual_system_step = LL_SYSTEM_STEP_GAME_RUN;
            }
			break;

		case LL_SYSTEM_STEP_GAME_RUN:
			// TODO: run the game and its logic
			// TODO: whence a player lost -> exit game
			ll_game_run(game);
			actual_system_step = LL_SYSTEM_STEP_GAME_EXIT;
			break;

		case LL_SYSTEM_STEP_GAME_PAUSE:
			// whence game is running and the switch was turned off
			// TODO: freeze game state and fade LED's
			actual_system_step = LL_SYSTEM_STEP_GAME_RUN;
			break;

		case LL_SYSTEM_STEP_GAME_EXIT:
			// after a game was lost or we got the switch combination
			// on -> off -> on -> off within 1 second
			// TODO: print some exit animation
			actual_system_step = LL_SYSTEM_STEP_STANDBY;
			break;

		case LL_SYSTEM_STEP_STANDBY:
			// TODO: print some standby animation
			actual_system_step = LL_SYSTEM_STEP_BOOT;
			break;

		case LL_SYSTEM_STEP_ERROR:
			break;

		case LL_SYSTEM_STEP_SHUTDOWN:
			break;
	}
}