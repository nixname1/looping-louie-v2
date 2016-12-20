#ifndef LL_ANIM_H_
#define LL_ANIM_H_

#include "stm32f4xx.h"

#define ANIMATION_FPS       (60)
#define ANIMATION_DELAY_MS  ((uint32_t) 1000/ANIMATION_FPS)

void ll_anim_animate_boot(void);

void ll_anim_stop_animation(void);

#endif
