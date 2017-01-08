#include "stm32f411xe.h"

#include "ll_player.h"
#include "ll_led.h"

/**
 * @brief returns the color for a specific player
 * @param player the requested player
 * @return the color
 */
struct color *ll_player_get_color(enum player player)
{
    static struct color c;
    switch(player)
    {
        case LL_PLAYER_0:
            c = YELLOW;
            break;
        case LL_PLAYER_1:
            c = PURPLE;
            break;
        case LL_PLAYER_2:
            c = RED;
            break;
        case LL_PLAYER_3:
            c = GREEN;
            break;
        case LL_PLAYER_4:
            c = YELLOW;
            break;
        case LL_PLAYER_5:
            c = PURPLE;
            break;
        case LL_PLAYER_6:
            c = RED;
            break;
        case LL_PLAYER_7:
            c = GREEN;
            break;
        case LL_PLAYER_ALL:
            c = WHITE;
            break;
    }
    return &c;
}

/**
 * @brief returns the color for a specific player.
 *          here player is a int, not a enum player
 *          if player is >7 value PLAYER_ALL is used
 * @param player the player
 * @return the color for the player
 */
struct color *ll_player_get_color_by_int(uint32_t player)
{
    switch(player)
    {
        case 0:
            return ll_player_get_color(LL_PLAYER_0);
        case 1:
            return ll_player_get_color(LL_PLAYER_1);
        case 2:
            return ll_player_get_color(LL_PLAYER_2);
        case 3:
            return ll_player_get_color(LL_PLAYER_3);
        case 4:
            return ll_player_get_color(LL_PLAYER_4);
        case 5:
            return ll_player_get_color(LL_PLAYER_5);
        case 6:
            return ll_player_get_color(LL_PLAYER_6);
        case 7:
            return ll_player_get_color(LL_PLAYER_7);
        default:
            return ll_player_get_color(LL_PLAYER_ALL);
    }
}