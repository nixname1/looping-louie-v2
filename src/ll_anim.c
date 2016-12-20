#include "stm32f4xx.h"
#include "ll_anim.h"
#include "ll_player.h"
#include "ll_led.h"

enum step
{
    LL_ANIM_STEP_START,
    LL_ANIM_STEP_RUN,
    LL_ANIM_STEP_FINISH,
    LL_ANIM_STEP_DO_NOTHING,
};

enum animation_step
{
    LL_ANIM_STEP_START_START,
    LL_ANIM_STEP_START_RUN,
    LL_ANIM_STEP_EXIT,
};

enum animation_state
{
    LL_ANIM_STATE_STARTING,
    LL_ANIM_STATE_RUNNING,
    LL_ANIM_STATE_FINISHED,
};

enum animation_type
{
    LL_ANIM_TYPE_BOOT,
};

struct animation
{
    int32_t player;
    enum animation_state state;
    enum animation_type type;
    enum animation_step start_step;
    enum animation_step run_step;
    enum animation_step exit_step;
    struct color pixel[LL_LED_NUM_PER_PLAYER];
};

uint32_t mg_stop_request = 0;
enum animation_state mg_state = LL_ANIM_STATE_FINISHED;
enum step mg_step = LL_ANIM_STEP_START;
struct animation mg_actual_animation;

uint32_t finish_animation(struct animation animation)
{
    switch(animation.type)
    {
        case LL_ANIM_TYPE_BOOT:

            break;
    }
}

uint32_t run_animation(struct animation animation)
{
    switch(animation.type)
    {
        case LL_ANIM_TYPE_BOOT:

            break;
    }
}

uint32_t start_animation(struct animation animation)
{
    switch(animation.type)
    {
        case LL_ANIM_TYPE_BOOT:

            break;
    }
}

/**
 * @brief runs the actual animation
 */
void ll_anim_run()
{
    uint32_t ret;
    switch(mg_step)
    {
        case LL_ANIM_STEP_START:
            if(mg_state == LL_ANIM_STATE_FINISHED)
            {
                ll_led_clear_all_pixel();
            }

            mg_state = LL_ANIM_STATE_STARTING;
            ret = start_animation(mg_actual_animation);
            if(ret)
            {
                mg_step = LL_ANIM_STEP_RUN;
            }
            break;

        case LL_ANIM_STEP_RUN:
            mg_state = LL_ANIM_STATE_RUNNING;
            ret = run_animation(mg_actual_animation);
            if(mg_stop_request && ret)
            {
                mg_step = LL_ANIM_STEP_FINISH;
            }
            break;

        case LL_ANIM_STEP_FINISH:
            ret = finish_animation(mg_actual_animation);
            if(ret)
            {
                mg_state = LL_ANIM_STATE_FINISHED;
                mg_step = LL_ANIM_STEP_DO_NOTHING;
            }
            break;

        case LL_ANIM_STEP_DO_NOTHING:
            // as said - do nothing
            break;
    }
}

/**
 * @brief stops the actual running animation
 */
void ll_anim_stop_animation(void)
{
    mg_stop_request = 1;
}

/**
 * @brief animate the boot animation
 */
void ll_anim_animate_boot(void)
{
    struct animation boot;
    boot.type = LL_ANIM_TYPE_BOOT;
    boot.player = LL_PLAYER_0;

    mg_step = LL_ANIM_STEP_START;
    mg_actual_animation = boot;
}