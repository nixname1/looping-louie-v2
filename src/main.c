#include <stdio.h>
#include <stdlib.h>

#include "stm32f4xx.h"

#include "diag/Trace.h"

#include "ll_motor.h"
#include "ll_system.h"
#include "ll_led.h"

enum ll_round_state handle_round(void);

/**
 * @brief handles the actual round step
 * @retval returns the actual state of the round
 */
enum ll_round_state handle_round()
{
    static enum ll_round_step actual_round_step = LL_STEP_ROUND_WAIT_FOR_START;
    enum ll_round_state retState = LL_STATE_ROUND_ERROR;
    switch (actual_round_step)
    {
        /**
         * wait for a push to the RS
         * NEXT: LL_STEP_ROUND_START
         */
        case LL_STEP_ROUND_WAIT_FOR_START:
            // TODO: play some little animation (with WS2812B and RS)
            // wait until RS was pressed
            actual_round_step = LL_STEP_ROUND_START;
            retState = LL_STATE_ROUND_WAITING;
            break;

            /**
             * if the RS were pushed and the last step was LL_STEP_ROUND_WAIT_FOR_START
             * NEXT: LL_STEP_ROUND_RUN
             */
        case LL_STEP_ROUND_START:
            // TODO: play animation for player that lost
            actual_round_step = LL_STEP_ROUND_RUN;
            retState = LL_STATE_ROUND_STARTING;
            break;

            /**
             * if a round is running
             * NEXT: LL_STEP_ROUND_END:
             */
        case LL_STEP_ROUND_RUN:
            // TODO: just some little animations (like single blinking LED's) and motor
            // controlling
            ll_motor_run();
            retState = LL_STATE_ROUND_RUNNING;
            break;

            /**
             * a player lost all chips -> round lost
             * NEXT: LL_STEP_ROUND_WAIT_FOR_START
             */
        case LL_STEP_ROUND_END:
            retState = LL_STATE_ROUND_STOPPING;
            // TODO: play animations; player that lost gets an own one
            break;

        case LL_STEP_ROUND_ERROR:
            // should not happen
            retState = LL_STATE_ROUND_ERROR;
            break;
    }

    return retState;
}

int main(int argc, char *argv[])
{
    UNUSED(argc);
    UNUSED(argv);

    enum ll_game_step actual_game_step = LL_STEP_RESET_AND_WAIT_FOR_START;
    enum ll_round_state round_state = LL_STATE_ROUND_ERROR;

    trace_printf("SystemCoreClock: %lu\n", SystemCoreClock);

    ll_system_init();

    struct color c = { 1, 2, 4 };
    while(1)
    {
        ll_led_write_pixel(c);
        delay_ms(1);
    }

    // FIXME: only for debug - until we have a reset switch
    actual_game_step = LL_STEP_GAME_RUN;

    while (1)
    {
        switch (actual_game_step)
        {
            /**
             * context:
             * "round" 	means the "actual playing" time. Everytime a player lost, a new
             * round starts.
             *
             * "game" 	a bunch of rounds. A round is started if the reset switch
             * 			is pressed first time after power on or after 10 minutes
             * (?)
             * 			the last round was lost by a player
             *
             * "play animation" play animation(s) with the WS2812B LED's
             */

            /**
             * wait for the beginning of a game. State after power on and after X
             * minutes not played
             * NEXT: LL_STEP_GAME_START
             */
            case LL_STEP_RESET_AND_WAIT_FOR_START:
                // TODO: fade LED of reset switch(RS) and play some nice animations with
                // the WS2812B
                break;

                /**
                 * the RS were pushed -> start a new game
                 * NEXT: LL_STEP_GAME_RUN
                 */
            case LL_STEP_GAME_START:
                // TODO: only light fading the RS-LED and play animation to start the game
                break;

                /**
                 * game is running -> handle round steps
                 * NEXT: -
                 */
            case LL_STEP_GAME_RUN:
                round_state = LL_STATE_ROUND_STARTING;
                while ((round_state != LL_STATE_ROUND_ERROR)
                       && (round_state != LL_STATE_ROUND_STOPPED))
                {
                    handle_round();
                }
                switch(round_state)
                {
                    case LL_STATE_ROUND_WAITING:
                    case LL_STATE_ROUND_STARTING:
                    case LL_STATE_ROUND_RUNNING:
                    case LL_STATE_ROUND_STOPPING:
                        break;
                    case LL_STATE_ROUND_STOPPED:
                        actual_game_step = LL_STEP_GAME_STOP;
                        break;
                    case LL_STATE_ROUND_ERROR:
                        actual_game_step = LL_STEP_GAME_ERROR;
                        break;
                }
                break;

                /**
                 * if after X minutes no new round were started
                 * or the RS were pushed for over 5 seconds
                 * NEXT: LL_STEP_RESET_AND_WAIT_FOR_START
                 */
            case LL_STEP_GAME_STOP:
                // TODO: stop the game, play an animation
                actual_game_step = LL_STEP_RESET_AND_WAIT_FOR_START;
                break;

                /**
                 * do not know if needed
                 * some error occurred
                 * NEXT: LL_STEP_RESET_AND_WAIT_FOR_START
                 */
            case LL_STEP_GAME_ERROR:
                // should not happen - idk if needed
                actual_game_step = LL_STEP_GAME_STOP;
                break;
        }
    }
}
