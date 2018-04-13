#ifndef LL_EXTERNAL_H_
#define LL_EXTERNAL_H_

#include "ll_player.h"
/**
 * There is a 74hc166 shift register.
 * The lightbarriers of all 8 players are connected to it.
 *
 *                    µC
 *                    |
 *                    V
 *  74hc166_1_D0: Lightbarrier for player 7
 *  74hc166_1_D1: Lightbarrier for player 6
 *  74hc166_1_D2: Lightbarrier for player 5
 *  74hc166_1_D3: Lightbarrier for player 4
 *  74hc166_1_D4: Lightbarrier for player 3
 *  74hc166_1_D5: Lightbarrier for player 2
 *  74hc166_1_D6: Lightbarrier for player 1
 *  74hc166_1_D7: Lightbarrier for player 0
 *                     |
 *                     V
 *                    GND
 */


/** get the status of external devices every X ms*/
#define LL_LB_POLL_TIME    (50)

/** number of devices connected to all 74hc166 */
#define LL_LB_COUNT     (LL_PLAYER_NUM_PLAYERS)

enum ll_lb_event_type
{
    LL_EXT_EVENT_START,
    LL_EXT_EVENT_END,
};

typedef void (*ll_lb_event_callback)(enum ll_lb_event_type, uint32_t lightbarrier, uint64_t event_time, void *payload);
typedef uint32_t (*ll_lb_state_update_cb)(void);

void ll_lb_init(ll_lb_state_update_cb update_cb, ll_lb_event_callback event_cb);
void ll_lb_run(uint64_t systime, void *payload);

uint32_t ll_lb_is_active(uint32_t device);

#endif
