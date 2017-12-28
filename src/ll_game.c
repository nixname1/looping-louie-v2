#include <stdlib.h>

#include "ll_anim.h"
#include "ll_switch.h"
#include "ll_lightbarrier.h"
#include "ll_motor.h"
#include "ll_game.h"

enum boot_step
{
	LL_BOOT_STEP_INIT,
	LL_BOOT_STEP_WAIT_FOR_SWITCH,
	LL_BOOT_STEP_WAIT_FOR_ANIMATION
};

enum round_result
{
	ROUND_RESULT_PLAYING,
	ROUND_RESULT_PLAYER_LOST,
	ROUND_RESULT_PAUSED
};

enum game_result
{
	GAME_RESULT_PLAYING,
	GAME_RESULT_PAUSED,
	GAME_RESULT_END
};

void ll_game_lb_event_callback(enum ll_lb_event_type event, uint32_t lightbarrier, uint64_t event_time, void *payload)
{
	struct game *game = payload;

	(void)(event_time);

	if(event == LL_EXT_EVENT_START)
	{
        if(game->state == LL_GAME_STATE_RUNNING && game->round_step == LL_ROUND_STEP_RUN)
        {
            if (game->player[lightbarrier].chips > 0)
            {
                game->player[lightbarrier].chips--;
            }
        }
	}
}

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
	if(game->round_counter == 0)
	{
		return 1;
	}

    for(uint32_t i = 0; i < game->player_count; i++)
    {
        game->player[i].chips = 3;
    }

	ll_anim_activate(LL_ANIM_ROUND_START);

    return 1;
}

static enum round_result run_round(struct game *game, uint64_t systime)
{
    enum round_result ret = ROUND_RESULT_PLAYING;

    ll_motor_run(systime);

	if(!ll_switch_is_turned_on())
	{
		return ROUND_RESULT_PAUSED;
	}

	if(!ll_anim_is_active())
	{
		ll_anim_activate(LL_ANIM_ROUND_RUN);
	}

	for(uint32_t i = 0; i < game->player_count; i++)
	{
		if(game->player[i].chips < 1)
		{
            game->player[i].lost_count++;
			game->player_lost = &game->player[i];
            ll_anim_activate(LL_ANIM_PLAYER_LOST);
			return ROUND_RESULT_PLAYER_LOST;
		}
	}

    return ret;
}

static uint32_t end_round(struct game *game)
{
    uint32_t ret = 0;
	if(!ll_switch_is_turned_on())
	{
		ret = 1;
		game->round_counter++;
		ll_anim_stop_animation();
	}
    return ret;
}

static uint32_t ll_game_start(struct game *game)
{
	static int is_first_call = 1;

	if(is_first_call)
	{
		game->state = LL_GAME_STATE_STARTING;
		ll_anim_activate(LL_ANIM_GAME_START);
		is_first_call = 0;
		return 0;
	}
	else
	{
		if(ll_anim_is_active())
		{
			return 0;
		}
		is_first_call = 1;
		return 1;
	}
}

static enum game_result ll_game_run(struct game *game, uint64_t systime)
{
	enum game_result ret = GAME_RESULT_PLAYING;
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
		    switch(run_round(game, systime))
		    {
			    case ROUND_RESULT_PLAYING:
				    break;

			    case ROUND_RESULT_PLAYER_LOST:
                    ll_motor_stop();
					game->round_step = LL_ROUND_STEP_END;
				    break;

			    case ROUND_RESULT_PAUSED:
                    ll_motor_stop();
				    ret = GAME_RESULT_PAUSED;
				    break;
		    }
            break;

        case LL_ROUND_STEP_END:
            if(end_round(game))
            {
                game->round_step = LL_ROUND_STEP_WAIT_FOR_START;
            }
            if(game->player_lost->lost_count >= LL_LED_NUM_PER_BAR)
            {
                return GAME_RESULT_END;
            }
            break;

		case LL_ROUND_STEP_WAIT_FOR_START:
			if(!ll_anim_is_active())
			{
				ll_anim_activate(LL_ANIM_ROUND_STANDBY);
			}
			if(ll_switch_is_turned_on())
			{
                game->round_step = LL_ROUND_STEP_START;
			}
			break;
    }

    return ret;
}

static uint32_t ll_game_pause(struct game *game)
{
	static uint32_t is_new_pause = 1;
	uint32_t ret = 0;

	game->state = LL_GAME_STATE_PAUSED;
	if(is_new_pause)
	{
        ll_anim_activate(LL_ANIM_GAME_PAUSE);
		is_new_pause = 0;
	}

	if(ll_switch_is_turned_on())
	{
		is_new_pause = 1;
		ret = 1;
		ll_anim_stop_animation();
	}
	return ret;
}

static uint32_t ll_game_end(struct game *game)
{
    if(!ll_switch_is_turned_on())
    {
        ll_anim_stop_animation();
        return 1;
    }
	game->state = LL_GAME_STATE_ENDED;
    return 0;
}

uint32_t ll_game_loop_run(struct game *game, uint64_t systime)
{
	ll_lb_run(systime, game);
	ll_anim_run(systime, game);

	switch (game->game_step)
	{
		case LL_GAME_STEP_BOOT:
			if(run_system_boot(systime))
				game->game_step = LL_GAME_STEP_WAIT_FOR_GAME_START;
			break;

		case LL_GAME_STEP_GAME_START:
			if(ll_game_start(game))
            {
	            game->game_step = LL_GAME_STEP_GAME_RUN;
            }
			break;

		case LL_GAME_STEP_GAME_RUN:
			switch(ll_game_run(game, systime))
			{
				case GAME_RESULT_PLAYING:
					break;

				case GAME_RESULT_PAUSED:
					game->game_step = LL_GAME_STEP_GAME_PAUSE;
					break;

				case GAME_RESULT_END:
					game->game_step = LL_GAME_STEP_GAME_EXIT;
					break;
			}
			break;

		case LL_GAME_STEP_GAME_PAUSE:
			if(ll_game_pause(game))
			{
				game->game_step = LL_GAME_STEP_GAME_RUN;
			}
			break;

		case LL_GAME_STEP_GAME_EXIT:
            if(ll_game_end(game))
            {
	            return 1;
            }
			break;

		case LL_GAME_STEP_WAIT_FOR_GAME_START:
			if(ll_switch_is_turned_on())
            {
	            game->game_step = LL_GAME_STEP_GAME_START;
            }
			break;

		case LL_GAME_STEP_ERROR:
			break;

		case LL_GAME_STEP_SHUTDOWN:
			break;
	}
	return 0;
}

struct game *ll_game_create()
{
	struct game *game = malloc(sizeof(*game));
	struct player *player = malloc(sizeof(*player) * LL_PLAYER_NUM_PLAYERS);

	if(!game || !player)
	{
		return NULL;
	}

	for(uint32_t i = 0; i < LL_PLAYER_NUM_PLAYERS; i++)
	{
		player[i].number = i;
		player[i].chips = 3;
		player[i].color = ll_player_get_color(i);
		player[i].lost_count = 0;
	}

	game->state = LL_GAME_STATE_STOPPED;
	game->game_step = LL_GAME_STEP_BOOT;
	game->round_step = LL_ROUND_STEP_START;
	game->motor_speed = 0;
	game->player = player;
	game->player_count = LL_PLAYER_NUM_PLAYERS;
	game->round_counter = 0;
	return game;
}
