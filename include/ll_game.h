#ifndef LL_GAME_H
#define LL_GAME_H

#include "ll_led.h"
#include "ll_lightbarrier.h"
#include "ll_player.h"

enum ll_game_state
{
	LL_GAME_STATE_STOPPED,
	LL_GAME_STATE_STARTING,
	LL_GAME_STATE_RUNNING,
	LL_GAME_STATE_ENDED,
	LL_GAME_STATE_PAUSED,
};

enum game_round_step
{
    LL_ROUND_STEP_START,
    LL_ROUND_STEP_RUN,
    LL_ROUND_STEP_END,
	LL_ROUND_STEP_WAIT_FOR_START
};

struct game
{
	uint32_t motor_speed;
	struct color *framebuffer;
	struct player *player;
	struct player *player_lost;
	uint32_t player_count;
	enum ll_game_state state;
	enum game_round_step round_step;
	uint8_t round_counter;
	uint8_t PADDING_3;
};

void ll_game_lb_event_callback(enum ll_lb_event_type event, uint32_t lightbarrier, uint64_t event_time, void *payload);

struct game *ll_game_create(struct player *player, uint32_t player_count);
void ll_game_loop_run(struct game *game, uint64_t systime);

#endif //LL_GAME_H
