#include <stdlib.h>
#include <memory.h>
#include "stm32f4xx.h"

#include "ll_anim.h"
#include "ll_led.h"
#include "ll_system.h"

#include "anim/system_boot.h"

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

uint32_t mg_animation_cnt = 0;
struct animation *mg_animations = NULL;

void ll_anim_init()
{
    anim_system_boot_init();
}

/**
 * @brief runs the actual animation
 */
void ll_anim_run() {
    static uint64_t last_update = 0;
    uint32_t ret;

    if (!mg_is_active)
        return;

    if(ll_system_get_systime() >= mg_last_run + mg_animations[mg_actual_animation].speed)
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
            ret = mg_animations[mg_actual_animation].start_animation(mg_animations[mg_actual_animation].payload);
            if (ret) {
                mg_step = LL_ANIM_STEP_RUN;
            }
            break;

        case LL_ANIM_STEP_RUN:
            mg_state = LL_ANIM_STATE_RUNNING;
            ret = mg_animations[mg_actual_animation].run_animation(mg_animations[mg_actual_animation].payload);
            if (mg_stop_request && ret) {
                mg_step = LL_ANIM_STEP_FINISH;
            }
            break;

        case LL_ANIM_STEP_FINISH:
            ret = mg_animations[mg_actual_animation].finish_animation(mg_animations[mg_actual_animation].payload);
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
    if (ll_system_get_systime() >= last_update + ANIMATION_DELAY_MS)
    {
        ll_led_update_all_leds();
        last_update = ll_system_get_systime();
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
}

int32_t ll_anim_add(animation_callback start_cb, animation_callback run_cb, animation_callback finish_cb,
                 uint32_t speed_fps, void *payload)
{
    mg_animation_cnt++;
    void *tmp;
    tmp = realloc(mg_animations, sizeof(*mg_animations) * mg_animation_cnt);
    if(tmp == NULL)
    {
        return -1;
    }

    mg_animations = tmp;
    mg_animations[mg_animation_cnt - 1].speed = 1000 / speed_fps;
    mg_animations[mg_animation_cnt - 1].payload = payload;
    mg_animations[mg_animation_cnt - 1].start_animation = start_cb;
    mg_animations[mg_animation_cnt - 1].run_animation = run_cb;
    mg_animations[mg_animation_cnt - 1].finish_animation = finish_cb;
    return mg_animation_cnt;
}
