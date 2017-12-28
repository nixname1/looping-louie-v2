#include <stdlib.h>

#include "ll_motor.h"

static uint64_t mg_next_speed_change = 0;

ll_motor_set_speed_cb mg_set_speed_cb = NULL;

void ll_motor_set_new_random_speed(void);

void ll_motor_init(ll_motor_set_speed_cb set_speed_cb)
{
    mg_set_speed_cb = set_speed_cb;
}

void ll_motor_set_new_random_speed(void)
{
    enum ll_motor_direction direction;
    int rand_tmp;
    uint32_t speed_percent;

    if(!mg_set_speed_cb)
    {
        return;
    }

    rand_tmp = random();
    // generate a direction
    if (rand_tmp >= ((RAND_MAX / 100) * 90)) // a change of 10% to get reverse
    {
        direction = LL_MOTOR_REVERSE;
    }
    else if (rand_tmp >= ((RAND_MAX / 100) * 70)) // a chance of 30% (minus the 10% from above) -> 20% for stop
    {
        direction = LL_MOTOR_STOP;
    }
    else
    {
        direction = LL_MOTOR_FORWARD;
    }

    // speed in precent from 1% to 100%
    speed_percent = ((uint32_t) random() % 100) + 1;
    mg_set_speed_cb(speed_percent, direction);
}

/**
 * @brief runs the motor
 */
void ll_motor_run(uint64_t systime)
{
    int rand_tmp;
    int hold_speed_for_ms; // time the motor holds the selected speed
    int t_min = 500;
    int t_max = 5500;
    int rnd_max = t_max;

    if (mg_next_speed_change <= systime)
    {
        ll_motor_set_new_random_speed();

        rand_tmp = (random() % rnd_max) + 1;

        if (rand_tmp >= (rnd_max / 100) * 80) // 20%
        {
            // |----- 10% ----|-------- 80% ---------|----- 10% ----|
            hold_speed_for_ms = (random() % 500) + 1;
            if ((random() % 2))
            {
                hold_speed_for_ms += t_min;
            }
            else
            {
                hold_speed_for_ms += (t_max - t_min);
            }
        }
        else
        {
            hold_speed_for_ms = (random() % (t_max - t_min - 500)) + t_min;
        }

        mg_next_speed_change = systime + (uint64_t) hold_speed_for_ms;
    }
}

/**
 * @brief stops the motor
 */
void ll_motor_stop()
{
    if(mg_set_speed_cb)
    {
        mg_set_speed_cb(0, LL_MOTOR_STOP);
    }
}
