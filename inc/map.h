#ifndef MAP_H
#define MAP_H

#include "defs.h"

/* Function prototypes */
bool load_map(game_t *game, const char *filename);
void render_minimap(game_t *game, int minimap_size);

#endif /* MAP_H */
