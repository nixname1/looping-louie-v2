#include <stdlib.h>

#include "stm32f4xx.h"
#include "diag/Trace.h"

#include "hardware/ll_system.h"
#include "ll_motor.h"

static uint64_t mg_next_speed_change = 0;

/**
 * @brief initializes the motor module
 * sets up a timer with output to a pin
 */
void ll_motor_init()
{
    /** initialize the GPIO Port */
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
    GPIOC->MODER |= GPIO_MODER_MODER6_1;
    GPIOC->OSPEEDR &= ~GPIO_OSPEEDER_OSPEEDR6;
    GPIOC->AFR[0] |= (GPIO_AF2_TIM3 << (6 * 4));

    /** initialize the timer */
    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
    TIM3->PSC = 96 - 1;	// 1000 clocks per ms
    TIM3->ARR = 20000 - 1; // 20ms per signal
    TIM3->CCMR1 = TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1; // pwm mode 1
    TIM3->CCER = TIM_CCER_CC1E; // enable ccr
    TIM3->CR1 |= TIM_CR1_CEN; // enable timer
}

/**
 * @brief
 * @param val	expects the speed of the motor in precent
 * @param dir	the direction of the motor
 * 				0=forward 1=backward
 * @retval		0 on success, -1 on failure
 */
int ll_motor_set_speed(uint32_t val, uint32_t dir)
{
    UNUSED(val);
    UNUSED(dir);

    return 0;
}

/**
 * @brief
 * @param val	expects the speed of the motor in precent
 * @param dir	the direction of the motor
 * 				0=forward 1=backward
 * @retval		0 on success, -1 on failure
 */
int ll_motor_set_new_random_speed()
{
    enum ll_motor_direction direction;
    uint32_t val = 0;
    int rand_tmp;
    int speed_precent;

    rand_tmp = rand();
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
    speed_precent = (rand() % 100) + 1;
    switch (direction)
    {
        case LL_MOTOR_REVERSE:
            // reverse starts at 1680. The reverse range is 380 steps
            // so calc: speed_precent * (step_range/100) to get the speed
            // in the reverse range. Then add 1680 as offset
            val = 1680 + (uint32_t) ((double) speed_precent * 3.8);
            trace_printf("setting speed: %u reverse\n", val);
            // use motor brake (~50% backward for ~10 ms)
            TIM3->CCR1 = 1850;
            delay_ms(100);
            // send zero
            TIM3->CCR1 = 1410;
            delay_ms(100);
            break;

        case LL_MOTOR_FORWARD:
            // same as in reverse expect the offset is 1180 and the range is 230
            val = 1180 + (uint32_t) ((double) speed_precent * 2.3);
            break;

        case LL_MOTOR_STOP:
            // in case of stop - set to ESC zero
            val = 1410;
            break;
    }
    TIM3->CCR1 = val;
    return 0;
}

/**
 * @brief runs the motor
 */
void ll_motor_run()
{
    int rand_tmp;
    int speed_time; // time the motor holds the selected speed
    int t_min = 500; // minimum ms
    int t_max = 5500; // maximum ms
    int rnd_max = t_max;

    if (mg_next_speed_change <= ll_system_get_systime())
    {
        ll_motor_set_new_random_speed();

        rand_tmp = (rand() % rnd_max) + 1;

        if (rand_tmp >= (rnd_max / 100) * 80) // 20%
        {
            // |- one second -|- t_max-t_min + 2sec -|- one second -|
            // |----- 10% ----|-------- 80% ---------|----- 10% ----|

            speed_time = (rand() % 500); // one second
            if ((rand() % 2))
            {
                speed_time += t_min; // low area
            }
            else
            {
                speed_time += (t_max - t_min); // high area
            }
        }
        else
        {
            //            random         range  - 8*500ms  + offset
            speed_time = (rand() % (t_max - t_min - 1000)) + t_min;
        }

        mg_next_speed_change = ll_system_get_systime()
                + (uint64_t) speed_time;
    }
}

/**
 * @brief stops the motor
 */
void ll_motor_stop()
{
    ll_motor_set_speed(0, 0);
}
