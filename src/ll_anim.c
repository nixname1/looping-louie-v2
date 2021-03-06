#include <stdlib.h>
#include <memory.h>

#include "hardware/ll_renderer.h"
#include "ll_anim.h"

enum animation_state
{
    LL_ANIM_STATE_STARTING,
    LL_ANIM_STATE_RUNNING,
    LL_ANIM_STATE_FINISHED,
};

uint32_t mg_stop_request = 0;
enum animation_state mg_state = LL_ANIM_STATE_FINISHED;
enum anim_step mg_step = LL_ANIM_STEP_START;
enum LL_ANIMATION mg_actual_animation;
uint32_t mg_is_active = 0;
uint64_t mg_last_run = 0;

struct animation mg_animations[LL_ANIM_NUM_ANIMATIONS];

render_frame_cb render_cb = NULL;

void ll_anim_init(render_frame_cb cb)
{
	render_cb = cb;
}

/**
 * @brief runs the actual animation
 */
void ll_anim_run(uint64_t system_time, struct game *game) {
	struct animation *anim = &mg_animations[mg_actual_animation];
    uint32_t ret;
    int render_request = 0;

    if (!mg_is_active)
        return;

    if(system_time >= mg_last_run + (1000/anim->speed))
    {
        mg_last_run = system_time;
    }
    else
    {
        return;
    }

    switch (mg_step) {
        case LL_ANIM_STEP_START:
            mg_state = LL_ANIM_STATE_STARTING;
            ret = anim->start_animation(anim->framebuffer, game, &render_request, anim->payload);
            if (ret) {
                mg_step = LL_ANIM_STEP_RUN;
            }
            break;

        case LL_ANIM_STEP_RUN:
            mg_state = LL_ANIM_STATE_RUNNING;
            ret = anim->run_animation(anim->framebuffer, game, &render_request, anim->payload);
            if (mg_stop_request && ret) {
                mg_step = LL_ANIM_STEP_FINISH;
            }
            break;

        case LL_ANIM_STEP_FINISH:
            ret = anim->finish_animation(anim->framebuffer, game, &render_request, anim->payload);
            if (ret) {
                mg_state = LL_ANIM_STATE_FINISHED;
                mg_step = LL_ANIM_STEP_DO_NOTHING;
                mg_is_active = 0;
                mg_stop_request = 0;
            }
            break;

        case LL_ANIM_STEP_DO_NOTHING:
            // as said - do nothing
            break;
    }

	if(render_request && render_cb)
	{
		render_cb(anim->framebuffer);
	}
}

/**
 * @brief   returns if an animation is active
 * @return  1 if is active, 0 if not
 */
uint32_t ll_anim_is_active()
{
    return mg_is_active;
}

/**
 * @brief stops the actual running animation
 */
void ll_anim_stop_animation(void)
{
    mg_stop_request = 1;
}

/**
 * @brief activates an animation
 * @param animation the animation to activate
 */
void ll_anim_activate(enum LL_ANIMATION animation)
{
    mg_actual_animation = animation;
    mg_step = LL_ANIM_STEP_START;
    mg_is_active = 1;
    mg_stop_request = (uint32_t) !mg_animations[animation].is_loop_animation;
}

int32_t ll_anim_add(enum LL_ANIMATION anim_name, struct animation *anim)
{
    if(!anim)
    {
        return -1;
    }

    memcpy(&mg_animations[anim_name], anim, sizeof(mg_animations[anim_name]));
    return 0;
}
