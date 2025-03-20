#ifndef UTILS_H
#define UTILS_H

#include "defs.h"

/* Function prototypes */
bool init_sdl(game_t *game);
void cleanup(game_t *game);
double get_delta_time(game_t *game);
void limit_fps(Uint32 frame_time);
void init_rain(game_t *game);
void update_rain(game_t *game);
void render_rain(game_t *game);

#endif /* UTILS_H */
