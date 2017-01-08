#include <stdlib.h>
#include <memory.h>
#include "stm32f4xx.h"

#include "ll_anim.h"
#include "ll_led.h"
#include "ll_system.h"

enum animation_state
{
    LL_ANIM_STATE_STARTING,
    LL_ANIM_STATE_RUNNING,
    LL_ANIM_STATE_FINISHED,
};

uint32_t mg_stop_request = 0;
enum animation_state mg_state = LL_ANIM_STATE_FINISHED;
enum anim_step mg_step = LL_ANIM_STEP_START;
struct animation mg_actual_animation;
uint32_t mg_is_active = 0;
uint64_t mg_last_run = 0;

/**
 * @brief runs the actual animation
 */
void ll_anim_run() {
    static uint64_t last_update = 0;
    uint32_t ret;

    if (!mg_is_active)
        return;

    if(ll_system_get_systime() >= mg_last_run + mg_actual_animation.speed)
    {
        mg_last_run = ll_system_get_systime();
    }
    else
    {
        return;
    }

    switch (mg_step) {
        case LL_ANIM_STEP_START:
            if (mg_state == LL_ANIM_STATE_FINISHED) {
                ll_led_clear_all_pixel();
            }

            mg_state = LL_ANIM_STATE_STARTING;
            ret = mg_actual_animation.start_animation(mg_actual_animation.payload);
            if (ret) {
                mg_step = LL_ANIM_STEP_RUN;
            }
            break;

        case LL_ANIM_STEP_RUN:
            mg_state = LL_ANIM_STATE_RUNNING;
            ret = mg_actual_animation.run_animation(mg_actual_animation.payload);
            if (mg_stop_request && ret) {
                mg_step = LL_ANIM_STEP_FINISH;
            }
            break;

        case LL_ANIM_STEP_FINISH:
            ret = mg_actual_animation.finish_animation(mg_actual_animation.payload);
            if (ret) {
                mg_state = LL_ANIM_STATE_FINISHED;
                mg_step = LL_ANIM_STEP_DO_NOTHING;
                mg_is_active = 0;
            }
            break;

        case LL_ANIM_STEP_DO_NOTHING:
            // as said - do nothing
            break;
    }
    if (ll_system_get_systime() >= last_update + ANIMATION_DELAY_MS)
    {
        ll_led_update_all_leds();
        last_update = ll_system_get_systime();
    }
}

/**
 * @brief   returns if a animation is active
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
void ll_anim_activate(struct animation *animation)
{
    memcpy(&mg_actual_animation, animation, sizeof(*animation));
    mg_step = LL_ANIM_STEP_START;
    mg_is_active = 1;
}
