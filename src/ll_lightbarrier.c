#include <stdint.h>
#include <string.h>

#include "ll_lightbarrier.h"

/** the last state of the external devices */
uint32_t old_state = 0;

/** the time of the last update */
uint64_t last_update_time = 0;

ll_lb_state_update_cb update_callback = NULL;
ll_lb_event_callback event_callback = NULL;

void ll_lb_init(ll_lb_state_update_cb update_cb, ll_lb_event_callback event_cb)
{
	update_callback = update_cb;
    event_callback = event_cb;
}

void ll_lb_run(uint64_t systime, void *payload)
{
    uint32_t new_state;

    if (systime < last_update_time + LL_LB_POLL_TIME)
    {
        return;
    }

    if(!update_callback || !event_callback)
    {
        return;
    }

    new_state = update_callback();
    for (uint32_t i = 0; i < LL_LB_COUNT; i++)
    {
        if (ll_lb_is_active(i))
        {
            if (!(new_state & ((uint32_t) 1 << i)))
            {
                event_callback(LL_EXT_EVENT_END, i, systime, payload);
            }
        } else
        {
            if ((new_state & ((uint32_t) 1 << i)))
            {
                event_callback(LL_EXT_EVENT_START, i, systime, payload);
            }
        }
    }
    old_state = new_state;

}

/**
 * @brief returns the status of a specific device
 * @retval 0 if device was not triggered; 1 if it was
 */
uint32_t ll_lb_is_active(uint32_t device)
{
    return (old_state & ((uint32_t) 1 << device)) >> device;
}
