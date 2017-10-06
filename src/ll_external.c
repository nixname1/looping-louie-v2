#include <stdint.h>
#include <string.h>

#include "ll_external.h"

/** the last state of the external devices */
uint32_t old_state = 0;

/** the callbacks for the devices */
ll_ext_device_event_callback mg_callbacks[LL_EXT_DEVICE_COUNT];

/** the time of the last update */
uint64_t last_update_time = 0;

ll_ext_state_update_cb update_callback = NULL;

void ll_ext_init(ll_ext_state_update_cb update_cb)
{
	update_callback = update_cb;
    memset(mg_callbacks, 0, sizeof(mg_callbacks)/sizeof(*mg_callbacks));
}

void ll_ext_run(uint64_t systime)
{
    uint32_t new_state;

    if (systime < last_update_time + LL_EXT_POLL_TIME)
    {
        return;
    }

    if(!update_callback)
    {
        return;
    }

    new_state = update_callback();
    for (enum ll_ext_device i = LL_EXT_DEVICE_LB_PLAYER0; i < LL_EXT_DEVICE_COUNT; i++)
    {
        if (mg_callbacks[i] == NULL)
        {
            continue;
        }

        if (ll_ext_is_device_active(i))
        {
            if (!(new_state & ((uint32_t) 1 << i)))
            {
                mg_callbacks[i](LL_EXT_EVENT_END, i, systime);
            }
        } else
        {
            if ((new_state & ((uint32_t) 1 << i)))
            {
                mg_callbacks[i](LL_EXT_EVENT_START, i, systime);
            }
        }
    }
    old_state = new_state;

}

/**
 * @brief adds a callback for a device that is conntected to the 74hc166
 * @param device    the device, the callback is for
 * @param callback  the callback to call on a event
 */
void ll_ext_set_event_callback(enum ll_ext_device device, ll_ext_device_event_callback callback)
{
    mg_callbacks[device] = callback;
}

/**
 * @brief returns the status of a specific device
 * @retval 0 if device was not triggered; 1 if it was
 */
uint32_t ll_ext_is_device_active(uint32_t device)
{
    return (old_state & ((uint32_t) 1 << device)) >> device;
}
