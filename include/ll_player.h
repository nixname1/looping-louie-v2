#ifndef LL_PLAYER_H_
#define LL_PLAYER_H_

#include <stdint.h>

#define LL_PLAYER_NUM_PLAYERS   8

struct player
{
    uint32_t number;
    uint32_t chips;
    uint32_t lost_count;
    const struct color *color;
};

struct color *ll_player_get_color(uint32_t player);

#endif
