#include <stdio.h>

#include "stm32f4xx.h"

#include "diag/Trace.h"

#include "ll_system.h"
#include "ll_external.h"
#include "ll_anim.h"
#include "ll_switch.h"

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

uint32_t run_system_boot()
{
    static enum boot_step step = LL_BOOT_STEP_INIT;
    static uint64_t boot_time = 0;

    switch(step)
    {
        case LL_BOOT_STEP_INIT:
            ll_anim_activate(LL_ANIM_SYSTEM_BOOT);
            boot_time = ll_system_get_systime();
            step = LL_BOOT_STEP_WAIT_FOR_SWITCH;
            break;

        case LL_BOOT_STEP_WAIT_FOR_SWITCH:
            if(ll_system_get_systime() >= boot_time + 3000 && !ll_switch_is_turned_on())
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


int main(int argc, char *argv[])
{
    UNUSED(argc);
    UNUSED(argv);
    enum ll_system_step actual_system_step;
    uint64_t boot_time = 0;

    trace_printf("SystemCoreClock: %lu\n", SystemCoreClock);

    ll_system_init();

    actual_system_step = LL_SYSTEM_STEP_BOOT;
    while (1)
    {
        ll_ext_run();
        ll_anim_run();

        switch (actual_system_step)
        {
            case LL_SYSTEM_STEP_BOOT:
                if(run_system_boot())
                    actual_system_step = LL_SYSTEM_STEP_STANDBY;
                break;

            case LL_SYSTEM_STEP_GAME_START:
                // TODO: print game start animation
                actual_system_step = LL_SYSTEM_STEP_GAME_RUN;
                break;

            case LL_SYSTEM_STEP_GAME_RUN:
                // TODO: run the game and its logic
                // TODO: whence a player lost -> exit game
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
}
