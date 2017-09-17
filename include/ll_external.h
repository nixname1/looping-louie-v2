#ifndef LL_EXTERNAL_H_
#define LL_EXTERNAL_H_

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
#define LL_EXT_POLL_TIME    (10)

/** number of devices connected to all 74hc166 */
#define LL_EXT_DEVICE_COUNT     (8)

/** time in ms from when a event gets interpreted as long event */
#define LL_EXT_LONG_EVENT_TIME  (3000)

/** defines for all devices */
enum ll_ext_device
{
    LL_EXT_DEVICE_LB_PLAYER7   = 0,
    LL_EXT_DEVICE_LB_PLAYER6   = 1,
    LL_EXT_DEVICE_LB_PLAYER5   = 2,
    LL_EXT_DEVICE_LB_PLAYER4   = 3,
    LL_EXT_DEVICE_LB_PLAYER3   = 4,
    LL_EXT_DEVICE_LB_PLAYER2   = 5,
    LL_EXT_DEVICE_LB_PLAYER1   = 6,
    LL_EXT_DEVICE_LB_PLAYER0   = 7
};

enum ll_ext_event
{
    /** gets fired as soon as a device sends its event (reading 1 from device) */
    LL_EXT_EVENT_START,
    /** gets fired if a device event is over (reading 0 from device) */
    LL_EXT_EVENT_END,
};

/** function type of callback */
typedef void (*ll_ext_device_event_callback)(enum ll_ext_event);

void ll_ext_init(void);
void ll_ext_run(void);

void ll_ext_set_event_callback(enum ll_ext_device device, ll_ext_device_event_callback callback);
uint32_t ll_ext_is_device_active(uint32_t device);
uint64_t ll_ext_get_last_readout_time(void);

#endif
