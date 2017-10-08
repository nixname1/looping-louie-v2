#ifndef LL_GAME_H
#define LL_GAME_H

#include "ll_led.h"

enum ll_game_state
{
	LL_GAME_STATE_STOPPED,
	LL_GAME_STATE_STARTING,
	LL_GAME_STATE_RUNNING,
	LL_GAME_STATE_STOPPING,
	LL_GAME_STATE_PAUSED,
};

struct game
{
	uint32_t motor_speed;
	struct color *framebuffer;
	struct player *player;
	uint32_t player_count;
	enum ll_game_state state;
	uint8_t PADDING_1;
	uint8_t PADDING_2;
	uint8_t PADDING_3;
};


void ll_game_loop_run(struct game *game, uint64_t systime);

void ll_game_lb_event_callback(enum ll_lb_event_type event, uint32_t lightbarrier, uint64_t event_time, void *payload);
/**
 * @brief creates a new game object
 * @return
 */
struct game *ll_game_create(struct player *player, uint32_t player_count);
/**
 * @brief starts a new game
 * @param game the game to start
 */
void ll_game_start(struct game *game);

/**
 * @brief runs the actual game
 * @param game the game to run
 */
void ll_game_run(struct game *game);

/**
 * @brief pauses the actual game
 * @param game the game to pause
 */
void ll_game_pause(struct game *game);

/**
 * @brief stop the actual game
 * @param game the game to stop
 */
void ll_game_stop(struct game *game);

#endif //LL_GAME_H
