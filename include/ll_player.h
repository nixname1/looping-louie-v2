#ifndef LL_PLAYER_H_
#define LL_PLAYER_H_

#include <stdint.h>

enum player
{
    LL_PLAYER_0 = 0,
    LL_PLAYER_1 = 1,
    LL_PLAYER_2 = 2,
    LL_PLAYER_3 = 3,
    LL_PLAYER_4 = 4,
    LL_PLAYER_5 = 5,
    LL_PLAYER_6 = 6,
    LL_PLAYER_7 = 7,
    LL_PLAYER_ALL,
};

#define LL_PLAYER_MAX_PLAYERS   8

struct color *ll_player_get_color(enum player player);
struct color *ll_player_get_color_by_int(uint32_t player);

#endif
