#include <stdio.h>
#include <stdlib.h>

#include "stm32f4xx.h"

#include "diag/Trace.h"

#include "ll_system.h"
#include "ll_motor.h"

int
main (int argc, char* argv[])
{
  UNUSED(argc);
  UNUSED(argv);

  enum LL_STEP actual_step = LL_STEP_RESET_AND_WAIT_FOR_START;
  uint32_t tmp = 0;

  trace_printf("SystemCoreClock: %lu\n", SystemCoreClock);

  ll_system_init();

  while (1)
  {
	switch(actual_step)
	{
	  /**
	   * context:
	   * "round" 	means the "actual playing" time. Everytime a player lost, a new round starts.
	   *
	   * "game" 	a bunch of rounds. A round is started if the reset switch
	   * 			is pressed first time after power on or after 10 minutes (?)
	   * 			the last round was lost by a player
	   *
	   * "play animation" play animation(s) with the WS2812B LED's
	   */

		/**
		 * wait for the beginning of a game. State after power on and after X minutes not played
		 * NEXT: LL_STEP_GAME_START
		 */
	  case LL_STEP_RESET_AND_WAIT_FOR_START:
		// TODO: fade LED of reset switch(RS) and play some nice animations with the WS2812B
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
		// TODO: handle the rounds (see below)
		break;

		/**
		 * if after X minutes no new round were started
		 * or the RS were pushed for over 5 seconds
		 * NEXT: LL_STEP_RESET_AND_WAIT_FOR_START
		 */
	  case LL_STEP_GAME_STOP:
		// TODO: stop the game, play an animation
		break;

		/**
		 * wait for a push to the RS
		 * NEXT: LL_STEP_ROUND_START
		 */
	  case LL_STEP_ROUND_WAIT_FOR_START:
		// TODO: play some little animation (with WS2812B and RS)
		break;

		/**
		 * if the RS were pushed and the last step was LL_STEP_ROUND_WAIT_FOR_START
		 * NEXT: LL_STEP_ROUND_RUN
		 */
	  case LL_STEP_ROUND_START:
		// TODO: play animation for player that lost
		break;

		/**
		 * if a round is running
		 * NEXT: LL_STEP_ROUND_END:
		 */
	  case LL_STEP_ROUND_RUN:
		// TODO: just some little animations (like single blinking LED's) and motor controlling

		break;

		/**
		 * a player lost all chips -> round lost
		 * NEXT: LL_STEP_ROUND_WAIT_FOR_START
		 */
	  case LL_STEP_ROUND_END:
		// TODO: play animations; player that lost gets an own one
		break;

		/**
		 * do not know if needed
		 * some error occurred
		 * NEXT: LL_STEP_RESET_AND_WAIT_FOR_START
		 */
	  case LL_STEP_ERROR:
		// should not happen - idk if needed
		break;

	  // this should never happen - otherwise something went terribly wrong!
	  default:
		break;
	}

  }

}
