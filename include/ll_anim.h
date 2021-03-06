#ifndef LL_ANIM_H_
#define LL_ANIM_H_

#include <stdint.h>
#include "ll_led.h"
#include "ll_game.h"

#define ANIMATION_FPS       (100)
#define ANIMATION_DELAY_MS  ((uint32_t) 1000/ANIMATION_FPS)

enum anim_step
{
    LL_ANIM_STEP_START,
    LL_ANIM_STEP_RUN,
    LL_ANIM_STEP_FINISH,
    LL_ANIM_STEP_DO_NOTHING,
};

typedef uint32_t (*animation_callback)(struct color *fb, struct game *game, int *render_request, void *payload);
typedef void (*render_frame_cb)(struct color *framebuffer);

struct animation
{
    uint32_t speed;
	uint32_t is_loop_animation;
    void *payload;
    animation_callback start_animation;
    animation_callback run_animation;
    animation_callback finish_animation;
	struct color *framebuffer;
};

enum LL_ANIMATION
{
    LL_ANIM_SYSTEM_BOOT = 0,
    //LL_ANIM_SYSTEM_ERROR = 1,
    LL_ANIM_GAME_START = 2,
    LL_ANIM_GAME_PAUSE = 3,
    LL_ANIM_GAME_EXIT = 4,
    LL_ANIM_PLAYER_LOST = 5,
    LL_ANIM_PLAYER_LOST_ALONE = 6,
	LL_ANIM_ROUND_START = 7,
	LL_ANIM_ROUND_RUN = 8,
	LL_ANIM_ROUND_WAIT_FOR_START = 9
};

#define LL_ANIM_NUM_ANIMATIONS (10)

void ll_anim_init(render_frame_cb cb);
void ll_anim_run(uint64_t system_time, struct game *game);

int32_t ll_anim_add(enum LL_ANIMATION anim_name, struct animation *anim);
void ll_anim_activate(enum LL_ANIMATION animation);
uint32_t ll_anim_is_active(void);
void ll_anim_stop_animation(void);

#endif
