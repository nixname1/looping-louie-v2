#include "ll_player.h"
#include "ll_led.h"

/**
 * @brief returns the color for a specific player.
 *          here player is a int, not a enum player
 *          if player is >7 value PLAYER_ALL is used
 * @param player the player
 * @return the color for the player
 */
const struct color *ll_player_get_color(uint32_t player)
{
    static struct color c;
    switch(player)
    {
        case 0:
            c = YELLOW;
            break;
        case 1:
            c = PURPLE;
            break;
        case 2:
            c = RED;
            break;
        case 3:
            c = GREEN;
            break;
        case 4:
            c = YELLOW;
            break;
        case 5:
            c = PURPLE;
            break;
        case 6:
            c = RED;
            break;
        case 7:
            c = GREEN;
            break;
        default:
            c = WHITE;
            break;
    }
    return &c;
}