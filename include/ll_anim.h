#ifndef LL_ANIM_H_
#define LL_ANIM_H_

#include "stm32f4xx.h"
#include "ll_player.h"

#define ANIMATION_FPS       (100)
#define ANIMATION_DELAY_MS  ((uint32_t) 1000/ANIMATION_FPS)

enum anim_step
{
    LL_ANIM_STEP_START,
    LL_ANIM_STEP_RUN,
    LL_ANIM_STEP_FINISH,
    LL_ANIM_STEP_DO_NOTHING,
};

typedef uint32_t (*animation_callback)(void *payload);

struct animation
{
    uint32_t speed;
    void *payload;
    animation_callback start_animation;
    animation_callback run_animation;
    animation_callback finish_animation;
};

void ll_anim_run(void);

void ll_anim_activate(struct animation *animation);
uint32_t ll_anim_is_active(void);
void ll_anim_stop_animation(void);

#endif
