#ifndef LL_EXTERNAL_H_
#define LL_EXTERNAL_H_

/**
 * There are 2 74hc166 ICs used. they are connected
 * in row to each other. The first one holds the light barriers,
 * the second one actually only the reset switch:
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
 *  74hc166_2_D0: Reset switch
 *  74hc166_2_D1-D7: GND
 */


/** get the status of external devices every 10 ms*/
#define LL_EXT_POLL_TIME    (10)

/** number of devices connected to all 74hc166 */
#define LL_EXT_DEVICE_COUNT     (9)

/** defines for all devices */
#define LL_EXT_POS_LB_PLAYER7   (0)
#define LL_EXT_POS_LB_PLAYER6   (1)
#define LL_EXT_POS_LB_PLAYER5   (2)
#define LL_EXT_POS_LB_PLAYER4   (3)
#define LL_EXT_POS_LB_PLAYER3   (4)
#define LL_EXT_POS_LB_PLAYER2   (5)
#define LL_EXT_POS_LB_PLAYER1   (6)
#define LL_EXT_POS_LB_PLAYER0   (7)
#define LL_EXT_POS_RESET_SWITCH (8)

void ll_ext_init(void);
void ll_ext_run(void);

uint32_t ll_ext_is_device_active(uint32_t device);

uint64_t ll_ext_get_last_readout_time(void);

#endif
