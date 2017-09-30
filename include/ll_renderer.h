#ifndef LL_RENDERER_H
#define LL_RENDERER_H

#include "ll_led.h"

void ll_renderer_init(void);
void ll_renderer_render_frame(struct color *framebuffer);

#endif
