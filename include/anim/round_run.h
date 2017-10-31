#ifndef ROUND_RUN_H
#define ROUND_RUN_H

#include "ll_led.h"
#include "ll_game.h"

struct animation *anim_round_run_init(struct color *framebuffer, struct game *game);

void ll_anim_round_run_generate_colors(struct color *framebuffer, struct game *game);

#endif
